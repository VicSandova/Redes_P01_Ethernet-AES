///*
// * Copyright (c) 2015, Freescale Semiconductor, Inc.
// * Copyright 2016-2020 NXP
// * All rights reserved.
// *
// * SPDX-License-Identifier: BSD-3-Clause
// */


//main.c:
#include <stdlib.h>
#include <string.h>
#include "enet.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_enet.h"
#include "fsl_phy.h"
#include "fsl_enet_mdio.h"
#include "fsl_phyksz8081.h"
#include "fsl_sysmpu.h"
#include "Aes_DR.h"

#define AES_BLOCK_SIZE 16

static const uint8_t aes_key[16] = {'M','y','1','6','b','y','t','e','K','e','y','0','0','0','0','0'};
static const uint8_t aes_iv[16]  = {'M','y','1','6','b','y','t','e','I','V','0','0','0','0','0','0'};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
size_t pad_pkcs7(uint8_t *data, size_t len);
void encrypt_and_send(const char *plaintext);
void receive_and_decrypt(uint8_t *frame, uint32_t length);

/*******************************************************************************
 * Code
 ******************************************************************************/
size_t pad_pkcs7(uint8_t *data, size_t len) {
    size_t pad_len = AES_BLOCK_SIZE - (len % AES_BLOCK_SIZE);
    for (size_t i = 0; i < pad_len; i++) {
        data[len + i] = pad_len;
    }
    return len + pad_len;
}



void encrypt_and_send(const char *plaintext) {
    struct AES_ctx ctx;
    uint8_t buffer[128] = {0};
    size_t len = strlen(plaintext);

    if (len == 0 || len > 112) {
        PRINTF("Invalid plaintext length: %d\r\n", len);
        return;
    }

    memcpy(buffer, plaintext, len);
    size_t padded_len = pad_pkcs7(buffer, len);

    AES_init_ctx_iv(&ctx, aes_key, aes_iv);
    AES_CBC_encrypt_buffer(&ctx, buffer, padded_len);

    // Dirección MAC destino
    g_frame[0] = 0x00;
    g_frame[1] = 0x90;
    g_frame[2] = 0x9E;
    g_frame[3] = 0x9A;
    g_frame[4] = 0x9F;
    g_frame[5] = 0x42;

    // Dirección MAC origen
    memcpy(&g_frame[6], &g_macAddr[0], 6);

    // Longitud del payload
    g_frame[12] = (uint8_t)(padded_len & 0xFF);
    g_frame[13] = (uint8_t)((padded_len >> 8) & 0xFF);


//    g_frame[12] = 0x88;
//    g_frame[13] = 0xB5; // Ethertype 0x88B5,

    // Copiar el mensaje cifrado
    memcpy(&g_frame[14], buffer, padded_len);

    //PRINTF("Payload length bytes: %02X %02X\r\n", g_frame[12], g_frame[13]);
    //PRINTF("Payload length: %d\r\n", padded_len);


    // Enviar el frame
    if (ENET_SendFrame(EXAMPLE_ENET, &g_handle, g_frame, padded_len + 14, 0, false, NULL) == kStatus_Success) {
        PRINTF("Encrypted frame sent!\r\n");
        //PRINTF("Padded length: %d\r\n", padded_len);

    } else {
        PRINTF("Failed to send encrypted frame!\r\n");
    }

    // Delay para adaptador
    for (volatile int i = 0; i < 10000000; i++) {
        asm("nop");
    }
}



// Funcion para desencriptar y mostrar mensaje recibido
void receive_and_decrypt(uint8_t *frame, uint32_t length)
{
    if (length < 14) {
        PRINTF("Frame demasiado corto\r\n");
        return;
    }

    // Extraer tamaño payload (bytes 12-13, little endian)
    uint16_t payload_len = frame[12] | (frame[13] << 8);

    if (payload_len == 0 || payload_len > (length - 14)) {
        PRINTF("Payload invalido: esperado %d, recibido %d\r\n",payload_len, length - 14);
        return;
    }

    if (payload_len % AES_BLOCK_SIZE != 0) {
        PRINTF("Payload no múltiplo de %d bytes, no se puede desencriptar.\r\n", AES_BLOCK_SIZE);
        return;
    }

    uint8_t buffer[1500] = {0};
    memcpy(buffer, &frame[14], payload_len);

    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, aes_key, aes_iv);

    AES_CBC_decrypt_buffer(&ctx, buffer, payload_len);


    uint8_t pad_len = buffer[payload_len - 1];
    if (pad_len > 0 && pad_len <= AES_BLOCK_SIZE) {
        uint8_t valid_pad = 1;
        for (int i = 0; i < pad_len; i++) {
            if (buffer[payload_len - 1 - i] != pad_len) {
                valid_pad = 0;
                break;
            }
        }
        if (!valid_pad) {
            PRINTF("Padding inválido.\r\n");
            return;
        }
    } else {
        PRINTF("Padding fuera de rango.\r\n");
        return;
    }

    // Imprimir mensaje desencriptado sin padding
    buffer[payload_len - pad_len] = '\0';  // Fin de cadena

    PRINTF("Mensaje desencriptado recibido: %s\r\n", buffer);
}



int main(void)
{
    uint32_t length;
    status_t status;
    bool link = false;

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    enet_init();

    while (!link) {
        PHY_GetLinkStatus(&phyHandle, &link);
    }



    while (1)
    {
    	//Tx
        encrypt_and_send("Aún hay esperanza...");


        // Rx
        status = ENET_GetRxFrameSize(&g_handle, &length, 0);
        if (length != 0) {
            uint8_t *data = (uint8_t *)malloc(length);
            status = ENET_ReadFrame(EXAMPLE_ENET, &g_handle, data, length, 0, NULL);
            if (status == kStatus_Success) {
                PRINTF("Frame received, length = %d\r\n", length);
                receive_and_decrypt(data, length);
            }
            free(data);
        } else if (status == kStatus_ENET_RxFrameError) {
            ENET_GetRxErrBeforeReadFrame(&g_handle, &eErrStatic, 0);
            ENET_ReadFrame(EXAMPLE_ENET, &g_handle, NULL, 0, 0, NULL);
        }
    }
}
