
//#include <stdlib.h>
//#include "pin_mux.h"
//#include "board.h"
//#include "fsl_debug_console.h"
//#include "fsl_enet.h"
//#include "fsl_phy.h"
//#include "fsl_enet_mdio.h"
//#include "fsl_phyksz8081.h"
//#include "fsl_sysmpu.h"
///*******************************************************************************
// * Definitions
// ******************************************************************************/
///* ENET base address */
//#define EXAMPLE_ENET        ENET
//#define EXAMPLE_PHY_ADDRESS 0x00U
//
///* MDIO operations. */
//#define EXAMPLE_MDIO_OPS enet_ops
///* PHY operations. */
//#define EXAMPLE_PHY_OPS phyksz8081_ops
///* ENET clock frequency. */
//#define EXAMPLE_CLOCK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)
//#define ENET_RXBD_NUM          (4)
//#define ENET_TXBD_NUM          (4)
//#define ENET_RXBUFF_SIZE       (ENET_FRAME_MAX_FRAMELEN)
//#define ENET_TXBUFF_SIZE       (ENET_FRAME_MAX_FRAMELEN)
//#define ENET_DATA_LENGTH       (64)
//#define ENET_TRANSMIT_DATA_NUM (20)
//#define APP_ENET_BUFF_ALIGNMENT ENET_BUFF_ALIGNMENT
//#define PHY_AUTONEGO_TIMEOUT_COUNT (100000)
//#define PHY_STABILITY_DELAY_US (0U)
//#define MAC_ADDRESS {0xd4, 0xbe, 0xd9, 0x45, 0x22, 0x60}
//
//
///*******************************************************************************
// * Prototypes
// ******************************************************************************/
//
///*! @brief Build ENET broadcast frame. */
//static void ENET_BuildBroadCastFrame(void);
//
///*******************************************************************************
// * Variables
// ******************************************************************************/
///*! @brief Buffer descriptors should be in non-cacheable region and should be align to "ENET_BUFF_ALIGNMENT". */
//AT_NONCACHEABLE_SECTION_ALIGN(enet_rx_bd_struct_t g_rxBuffDescrip[ENET_RXBD_NUM], ENET_BUFF_ALIGNMENT);
//AT_NONCACHEABLE_SECTION_ALIGN(enet_tx_bd_struct_t g_txBuffDescrip[ENET_TXBD_NUM], ENET_BUFF_ALIGNMENT);
///*! @brief The data buffers can be in cacheable region or in non-cacheable region.
// * If use cacheable region, the alignment size should be the maximum size of "CACHE LINE SIZE" and "ENET_BUFF_ALIGNMENT"
// * If use non-cache region, the alignment size is the "ENET_BUFF_ALIGNMENT".
// */
//SDK_ALIGN(uint8_t g_rxDataBuff[ENET_RXBD_NUM][SDK_SIZEALIGN(ENET_RXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT)],
//          APP_ENET_BUFF_ALIGNMENT);
//SDK_ALIGN(uint8_t g_txDataBuff[ENET_TXBD_NUM][SDK_SIZEALIGN(ENET_TXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT)],
//          APP_ENET_BUFF_ALIGNMENT);
//
//enet_handle_t g_handle;
//uint8_t g_frame[ENET_DATA_LENGTH + 14];
//
///*! @brief The MAC address for ENET device. */
//uint8_t g_macAddr[6] = MAC_ADDRESS;
//
///*! @brief Enet PHY and MDIO interface handler. */
//static mdio_handle_t mdioHandle = {.ops = &EXAMPLE_MDIO_OPS};
//static phy_handle_t phyHandle   = {.phyAddr = EXAMPLE_PHY_ADDRESS, .mdioHandle = &mdioHandle, .ops = &EXAMPLE_PHY_OPS};
//
///*******************************************************************************
// * Code
// ******************************************************************************/
///*! @brief Build Frame for transmit. */
//static void ENET_BuildBroadCastFrame(void)
//{
//    uint32_t count  = 0;
////    uint32_t length = ENET_DATA_LENGTH - 14;
//    uint32_t length = 64;
//
////    for (count = 0; count < 6U; count++)
////    {
////        g_frame[count] = 0xFFU;
////    }
//
//    //Mi direccion MAC
//
//    g_frame[0] = 0x10U;
//    g_frame[1] = 0x63U;
//    g_frame[2] = 0xC8U;
//    g_frame[3] = 0xF7U;
//    g_frame[4] = 0x6AU;
//    g_frame[5] = 0x63U;
//
//    memcpy(&g_frame[6], &g_macAddr[0], 6U);
//    g_frame[12] = (length >> 8) & 0xFFU;
//    g_frame[13] = length & 0xFFU;
//
////    for (count = 0; count < length; count++)
////    {
////        g_frame[count + 14] = count % 0xFFU;
////    }
//    g_frame[14] = 0x43U; // 'C'
//    g_frame[15] = 0x61U; // 'a'
//    g_frame[16] = 0x72U; // 'r'
//    g_frame[17] = 0x6cU; // 'l'
//    g_frame[18] = 0x6fU; // 'o'
//    g_frame[19] = 0x73U; // 's'
//    g_frame[20] = 0x20U; // ' '
//    g_frame[21] = 0x45U; // 'E'
//    g_frame[22] = 0x6dU; // 'm'
//    g_frame[23] = 0x69U; // 'i'
//    g_frame[24] = 0x6cU; // 'l'
//    g_frame[25] = 0x69U; // 'i'
//    g_frame[26] = 0x6fU; // 'o'
//
//        for (count = 1; count < length + 26; count++)
//        {
//            g_frame[count + 26] = count % 0xFFU;
//        }
//
//}
//
///*!
// * @brief Main function
// */
//int main(void)
//{
//    enet_config_t config;
//    phy_config_t phyConfig = {0};
//    uint32_t length        = 0;
//    bool link              = false;
//    bool autonego          = false;
//    phy_speed_t speed;
//    phy_duplex_t duplex;
//    uint32_t testTxNum = 0;
//    status_t status;
//    enet_data_error_stats_t eErrStatic;
//    volatile uint32_t count = 0;
//
//    /* Hardware Initialization. */
//    BOARD_InitBootPins();
//    BOARD_InitBootClocks();
//    BOARD_InitDebugConsole();
//
//    /* Disable SYSMPU. */
//    SYSMPU_Enable(SYSMPU, false);
//    /* Set RMII clock src. */
//    CLOCK_SetRmii0Clock(1U);
//
//    PRINTF("\r\nENET example start.\r\n");
//
//    /* Prepare the buffer configuration. */
//    enet_buffer_config_t buffConfig[] = {{
//        ENET_RXBD_NUM,
//        ENET_TXBD_NUM,
//        SDK_SIZEALIGN(ENET_RXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT),
//        SDK_SIZEALIGN(ENET_TXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT),
//        &g_rxBuffDescrip[0],
//        &g_txBuffDescrip[0],
//        &g_rxDataBuff[0][0],
//        &g_txDataBuff[0][0],
//        true,
//        true,
//        NULL,
//    }};
//
//
//    ENET_GetDefaultConfig(&config);
//
//    /* The miiMode should be set according to the different PHY interfaces. */
//#ifdef EXAMPLE_PHY_INTERFACE_RGMII
//    config.miiMode = kENET_RgmiiMode;
//#else
//    config.miiMode = kENET_RmiiMode;
//#endif
//    phyConfig.phyAddr               = EXAMPLE_PHY_ADDRESS;
//    phyConfig.autoNeg               = true;
//    mdioHandle.resource.base        = EXAMPLE_ENET;
//    mdioHandle.resource.csrClock_Hz = EXAMPLE_CLOCK_FREQ;
//
//    /* Initialize PHY and wait auto-negotiation over. */
//    PRINTF("Wait for PHY init...\r\n");
//    do
//    {
//        status = PHY_Init(&phyHandle, &phyConfig);
//        if (status == kStatus_Success)
//        {
//            PRINTF("Wait for PHY link up...\r\n");
//            /* Wait for auto-negotiation success and link up */
//            count = PHY_AUTONEGO_TIMEOUT_COUNT;
//            do
//            {
//                PHY_GetAutoNegotiationStatus(&phyHandle, &autonego);
//                PHY_GetLinkStatus(&phyHandle, &link);
//                if (autonego && link)
//                {
//                    break;
//                }
//            } while (--count);
//            if (!autonego)
//            {
//                PRINTF("PHY Auto-negotiation failed. Please check the cable connection and link partner setting.\r\n");
//            }
//        }
//    } while (!(link && autonego));
//
//#if PHY_STABILITY_DELAY_US
//    /* Wait a moment for PHY status to be stable. */
//    SDK_DelayAtLeastUs(PHY_STABILITY_DELAY_US, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
//#endif
//
//    /* Get the actual PHY link speed. */
//    PHY_GetLinkSpeedDuplex(&phyHandle, &speed, &duplex);
//    /* Change the MII speed and duplex for actual link status. */
//    config.miiSpeed  = (enet_mii_speed_t)speed;
//    config.miiDuplex = (enet_mii_duplex_t)duplex;
//
//    ENET_Init(EXAMPLE_ENET, &g_handle, &config, &buffConfig[0], &g_macAddr[0], EXAMPLE_CLOCK_FREQ);
//    ENET_ActiveRead(EXAMPLE_ENET);
//
//    /* Build broadcast for sending. */
//    ENET_BuildBroadCastFrame();
//
//    while (1)
//    {
//        /* Get the Frame size */
//        status = ENET_GetRxFrameSize(&g_handle, &length, 0);
//        /* Call ENET_ReadFrame when there is a received frame. */
//        if (length != 0)
//        {
//            /* Received valid frame. Deliver the rx buffer with the size equal to length. */
//            uint8_t *data = (uint8_t *)malloc(length);
//            status        = ENET_ReadFrame(EXAMPLE_ENET, &g_handle, data, length, 0, NULL);
//            if (status == kStatus_Success)
//            {
//                PRINTF(" A frame received. the length %d ", length);
//                PRINTF(" Dest Address %02x:%02x:%02x:%02x:%02x:%02x Src Address %02x:%02x:%02x:%02x:%02x:%02x \r\n",
//                       data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9],
//                       data[10], data[11]);
//            }
//            free(data);
//        }
//        else if (status == kStatus_ENET_RxFrameError)
//        {
//            /* Update the received buffer when error happened. */
//            /* Get the error information of the received g_frame. */
//            ENET_GetRxErrBeforeReadFrame(&g_handle, &eErrStatic, 0);
//            /* update the receive buffer. */
//            ENET_ReadFrame(EXAMPLE_ENET, &g_handle, NULL, 0, 0, NULL);
//        }
//
//        if (testTxNum < ENET_TRANSMIT_DATA_NUM)
//        {
//            /* Send a multicast frame when the PHY is link up. */
//            if (kStatus_Success == PHY_GetLinkStatus(&phyHandle, &link))
//            {
//                if (link)
//                {
//                    testTxNum++;
//                    if (kStatus_Success ==
//                        ENET_SendFrame(EXAMPLE_ENET, &g_handle, &g_frame[0], ENET_DATA_LENGTH, 0, false, NULL))
//                    {
//                    	for(int i = 0;i < 10000000;i++){
//                    		asm("nop");
//                    	}
//
//                        PRINTF("The %d frame transmitted success!\r\n", testTxNum);
//                    }
//                    else
//                    {
//                        PRINTF(" \r\nTransmit frame failed!\r\n");
//                    }
//                }
//            }
//        }
//    }
//}


////main.c:
//#include <stdlib.h>
//#include "enet.h"
//#include "pin_mux.h"
//#include "board.h"
//#include "fsl_debug_console.h"
//#include "fsl_enet.h"
//#include "fsl_phy.h"
//#include "fsl_enet_mdio.h"
//#include "fsl_phyksz8081.h"
//#include "fsl_sysmpu.h"
//
//
//#define AES_BLOCK_SIZE 16
//
//static const uint8_t aes_key[16] = {'M','y','1','6','b','y','t','e','K','e','y','0','0','0','0','0'};
//static const uint8_t aes_iv[16]  = {'M','y','1','6','b','y','t','e','I','V','0','0','0','0','0','0'};
//
//
///*******************************************************************************
// * Prototypes
// ******************************************************************************/
//
//size_t pad_pkcs7(uint8_t *data, size_t len);
//void encrypt_and_send(const char *plaintext);
///*! @brief Build ENET broadcast frame. */
//static void ENET_BuildBroadCastFrame(void);
//
///*******************************************************************************
// * Variables
// ******************************************************************************/
//
//
//
///*******************************************************************************
// * Code
// ******************************************************************************/
///*! @brief Build Frame for transmit. */
//static void ENET_BuildBroadCastFrame(void)
//{
//    uint32_t count  = 0;
////    uint32_t length = ENET_DATA_LENGTH - 14;
//    uint32_t length = 64;
//
////    for (count = 0; count < 6U; count++)
////    {
////        g_frame[count] = 0xFFU;
////    }
//
//    //Mi direccion MAC
//
//    g_frame[0] = 0x10U;
//    g_frame[1] = 0x63U;
//    g_frame[2] = 0xC8U;
//    g_frame[3] = 0xF7U;
//    g_frame[4] = 0x6AU;
//    g_frame[5] = 0x63U;
//
//    memcpy(&g_frame[6], &g_macAddr[0], 6U);
//    g_frame[12] = (length >> 8) & 0xFFU;
//    g_frame[13] = length & 0xFFU;
//
////    for (count = 0; count < length; count++)
////    {
////        g_frame[count + 14] = count % 0xFFU;
////    }
//    g_frame[14] = 0x43U; // 'C'
//    g_frame[15] = 0x61U; // 'a'
//    g_frame[16] = 0x72U; // 'r'
//    g_frame[17] = 0x6cU; // 'l'
//    g_frame[18] = 0x6fU; // 'o'
//    g_frame[19] = 0x73U; // 's'
//    g_frame[20] = 0x20U; // ' '
//    g_frame[21] = 0x45U; // 'E'
//    g_frame[22] = 0x6dU; // 'm'
//    g_frame[23] = 0x69U; // 'i'
//    g_frame[24] = 0x6cU; // 'l'
//    g_frame[25] = 0x69U; // 'i'
//    g_frame[26] = 0x6fU; // 'o'
//
//        for (count = 1; count < length + 26; count++)
//        {
//            g_frame[count + 26] = count % 0xFFU;
//        }
//
//}
//
///*!
// * @brief Main function
// */
//int main(void)
//{
//
//
//    /* Hardware Initialization. */
//    BOARD_InitBootPins();
//    BOARD_InitBootClocks();
//    BOARD_InitDebugConsole();
//    enet_init();
//
//    /* Build broadcast for sending. */
//    ENET_BuildBroadCastFrame();
//
//    while (1)
//    {
//        /* Get the Frame size */
//        status = ENET_GetRxFrameSize(&g_handle, &length, 0);
//        /* Call ENET_ReadFrame when there is a received frame. */
//        if (length != 0)
//        {
//            /* Received valid frame. Deliver the rx buffer with the size equal to length. */
//            uint8_t *data = (uint8_t *)malloc(length);
//            status        = ENET_ReadFrame(EXAMPLE_ENET, &g_handle, data, length, 0, NULL);
//            if (status == kStatus_Success)
//            {
//                PRINTF(" A frame received. the length %d ", length);
//                PRINTF(" Dest Address %02x:%02x:%02x:%02x:%02x:%02x Src Address %02x:%02x:%02x:%02x:%02x:%02x \r\n",
//                       data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9],
//                       data[10], data[11]);
//            }
//            free(data);
//        }
//        else if (status == kStatus_ENET_RxFrameError)
//        {
//            /* Update the received buffer when error happened. */
//            /* Get the error information of the received g_frame. */
//            ENET_GetRxErrBeforeReadFrame(&g_handle, &eErrStatic, 0);
//            /* update the receive buffer. */
//            ENET_ReadFrame(EXAMPLE_ENET, &g_handle, NULL, 0, 0, NULL);
//        }
//
//        if (testTxNum < ENET_TRANSMIT_DATA_NUM)
//        {
//            /* Send a multicast frame when the PHY is link up. */
//            if (kStatus_Success == PHY_GetLinkStatus(&phyHandle, &link))
//            {
//                if (link)
//                {
//                    testTxNum++;
//                    if (kStatus_Success ==
//                        ENET_SendFrame(EXAMPLE_ENET, &g_handle, &g_frame[0], ENET_DATA_LENGTH, 0, false, NULL))
//                    {
//                    	for(int i = 0;i < 10000000;i++){
//                    		asm("nop");
//                    	}
//
//                        PRINTF("The %d frame transmitted success!\r\n", testTxNum);
//                    }
//                    else
//                    {
//                        PRINTF(" \r\nTransmit frame failed!\r\n");
//                    }
//                }
//            }
//        }
//    }
//}
//
//
//size_t pad_pkcs7(uint8_t *data, size_t len) {
//    size_t pad_len = AES_BLOCK_SIZE - (len % AES_BLOCK_SIZE);
//    for (size_t i = 0; i < pad_len; i++) {
//        data[len + i] = pad_len;
//    }
//    return len + pad_len;
//}
//
//void encrypt_and_send(const char *plaintext) {
//    struct AES_ctx ctx;
//    uint8_t buffer[128] = {0};  // Suponiendo máximo 112 bytes de texto plano
//    size_t len = strlen(plaintext);
//
//    if (len > 112) return;  // Evitar overflow (112 + 16 = 128 max)
//
//    memcpy(buffer, plaintext, len);
//    size_t padded_len = pad_pkcs7(buffer, len);
//
//    AES_init_ctx_iv(&ctx, aes_key, aes_iv);
//    AES_CBC_encrypt_buffer(&ctx, buffer, padded_len);
//
//    // Construir el frame Ethernet con datos cifrados
//    uint32_t count;
//    for (count = 0; count < 6U; count++) {
//        g_frame[count] = 0x10U;  // Dirección MAC destino: PC
//    }
//
//    memcpy(&g_frame[6], &g_macAddr[0], 6U);  // MAC origen: FRDM
//    g_frame[12] = (padded_len >> 8) & 0xFFU;
//    g_frame[13] = padded_len & 0xFFU;
//
//    memcpy(&g_frame[14], buffer, padded_len);
//
//    if (kStatus_Success == ENET_SendFrame(EXAMPLE_ENET, &g_handle, &g_frame[0], padded_len + 14, 0, false, NULL)) {
//        PRINTF("Encrypted frame sent!\r\n");
//    } else {
//        PRINTF("Failed to send encrypted frame!\r\n");
//    }
//}
//
//
////main.c:
//#include <stdlib.h>
//#include <string.h>
//#include "enet.h"
//#include "pin_mux.h"
//#include "board.h"
//#include "fsl_debug_console.h"
//#include "fsl_enet.h"
//#include "fsl_phy.h"
//#include "fsl_enet_mdio.h"
//#include "fsl_phyksz8081.h"
//#include "fsl_sysmpu.h"
//#include "Aes_DR.h"
//
//#define AES_BLOCK_SIZE 16
//
//static const uint8_t aes_key[16] = {'M','y','1','6','b','y','t','e','K','e','y','0','0','0','0','0'};
//static const uint8_t aes_iv[16]  = {'M','y','1','6','b','y','t','e','I','V','0','0','0','0','0','0'};
//
///*******************************************************************************
// * Prototypes
// ******************************************************************************/
//size_t pad_pkcs7(uint8_t *data, size_t len);
//void encrypt_and_send(const char *plaintext);
//
///*******************************************************************************
// * Code
// ******************************************************************************/
//size_t pad_pkcs7(uint8_t *data, size_t len) {
//    size_t pad_len = AES_BLOCK_SIZE - (len % AES_BLOCK_SIZE);
//    for (size_t i = 0; i < pad_len; i++) {
//        data[len + i] = pad_len;
//    }
//    return len + pad_len;
//}
//
//
//
//void encrypt_and_send(const char *plaintext) {
//    struct AES_ctx ctx;
//    uint8_t buffer[128] = {0};  // Máximo 112 + padding
//    size_t len = strlen(plaintext);
//
//    if (len == 0 || len > 112) {
//        PRINTF("Invalid plaintext length: %d\r\n", len);
//        return;
//    }
//
//    memcpy(buffer, plaintext, len);
//    size_t padded_len = pad_pkcs7(buffer, len);
//
//    AES_init_ctx_iv(&ctx, aes_key, aes_iv);
//    AES_CBC_encrypt_buffer(&ctx, buffer, padded_len);
//
//    // Dirección MAC destino = tu PC: 10:63:C8:F7:6A:64
//    g_frame[0] = 0x00;
//    g_frame[1] = 0x90;
//    g_frame[2] = 0x9E;
//    g_frame[3] = 0x9A;
//    g_frame[4] = 0x9F;
//    g_frame[5] = 0x42;
//
//    // Dirección MAC origen (debe estar inicializada correctamente en g_macAddr)
//    memcpy(&g_frame[6], &g_macAddr[0], 6);
//
//    // Longitud del payload (sin tipo, estilo Ethernet II)
//    g_frame[12] = (uint8_t)(padded_len & 0xFF);
//    g_frame[13] = (uint8_t)((padded_len >> 8) & 0xFF);
//
//
////    g_frame[12] = 0x88;
////    g_frame[13] = 0xB5; // Ethertype 0x88B5, experimental, válido
//    // Copiar el mensaje cifrado
//    memcpy(&g_frame[14], buffer, padded_len);
//
//    PRINTF("Payload length bytes: %02X %02X\r\n", g_frame[12], g_frame[13]);
//    PRINTF("Payload length: %d\r\n", padded_len);
//
//
//    // Enviar el frame
//    if (ENET_SendFrame(EXAMPLE_ENET, &g_handle, g_frame, padded_len + 14, 0, false, NULL) == kStatus_Success) {
//        PRINTF("Encrypted frame sent!\r\n");
//        PRINTF("Padded length: %d\r\n", padded_len);
//
//    } else {
//        PRINTF("Failed to send encrypted frame!\r\n");
//    }
//
//    // Delay para evitar saturación si es USB-Ethernet
//    for (volatile int i = 0; i < 10000000; i++) {
//        asm("nop");
//    }
//}
//
//#include <stdio.h>
//#include <string.h>
//#include "Aes_DR.h"  // Tu librería AES
//#include "fsl_debug_console.h"
//
//#define AES_BLOCK_SIZE 16
//
//// Función para desencriptar y mostrar mensaje recibido
//void receive_and_decrypt(uint8_t *frame, uint32_t length)
//{
//    if (length < 14) {
//        PRINTF("Frame demasiado corto para contener header Ethernet.\r\n");
//        return;
//    }
//
//    // Extraer tamaño payload (bytes 12-13, little endian)
//    uint16_t payload_len = frame[12] | (frame[13] << 8);
//
//    if (payload_len == 0 || payload_len > (length - 14)) {
//        PRINTF("Payload inválido o tamaño no coincide: esperado %d, recibido %d\r\n",
//               payload_len, length - 14);
//        return;
//    }
//
//    if (payload_len % AES_BLOCK_SIZE != 0) {
//        PRINTF("Payload no múltiplo de %d bytes, no se puede desencriptar.\r\n", AES_BLOCK_SIZE);
//        return;
//    }
//
//    uint8_t buffer[1500] = {0};  // Suficiente para payload máximo
//    memcpy(buffer, &frame[14], payload_len);
//
//    struct AES_ctx ctx;
//    AES_init_ctx_iv(&ctx, aes_key, aes_iv);
//
//    AES_CBC_decrypt_buffer(&ctx, buffer, payload_len);
//
//    // El mensaje está con padding PKCS7, removerlo
//    uint8_t pad_len = buffer[payload_len - 1];
//    if (pad_len > 0 && pad_len <= AES_BLOCK_SIZE) {
//        // Validar padding (opcional)
//        uint8_t valid_pad = 1;
//        for (int i = 0; i < pad_len; i++) {
//            if (buffer[payload_len - 1 - i] != pad_len) {
//                valid_pad = 0;
//                break;
//            }
//        }
//        if (!valid_pad) {
//            PRINTF("Padding inválido.\r\n");
//            return;
//        }
//    } else {
//        PRINTF("Padding fuera de rango.\r\n");
//        return;
//    }
//
//    // Imprimir mensaje desencriptado sin padding
//    buffer[payload_len - pad_len] = '\0';  // Fin de cadena
//
//    PRINTF("Mensaje desencriptado recibido: %s\r\n", buffer);
//}
//
//
//
//int main(void)
//{
//    uint32_t length;
//    status_t status;
//    bool link = false;
//
//    BOARD_InitBootPins();
//    BOARD_InitBootClocks();
//    BOARD_InitDebugConsole();
//    enet_init();
//
//    // Espera al enlace antes de enviar
//    while (!link) {
//        PHY_GetLinkStatus(&phyHandle, &link);
//    }
//
//    // Envía texto cifrado
//    //encrypt_and_send("No todo lo que es oro reluce...");
//
//    while (1)
//    {
//        // Envía texto cifrado
//        encrypt_and_send("Aún en la oscuridad...");
//
//
//        // Recepción pasiva
//        status = ENET_GetRxFrameSize(&g_handle, &length, 0);
//        if (length != 0) {
//            uint8_t *data = (uint8_t *)malloc(length);
//            status = ENET_ReadFrame(EXAMPLE_ENET, &g_handle, data, length, 0, NULL);
//            if (status == kStatus_Success) {
//                PRINTF("Frame received, length = %d\r\n", length);
//                receive_and_decrypt(data, length);
//            }
//            free(data);
//        } else if (status == kStatus_ENET_RxFrameError) {
//            ENET_GetRxErrBeforeReadFrame(&g_handle, &eErrStatic, 0);
//            ENET_ReadFrame(EXAMPLE_ENET, &g_handle, NULL, 0, 0, NULL);
//        }
//    }
//}
//void encrypt_and_send(const char *plaintext) {
//    struct AES_ctx ctx;
//    uint8_t buffer[128] = {0};  // Máximo 112 bytes de texto plano
//    size_t len = strlen(plaintext);
//    if (len > 112) return;
//
//    memcpy(buffer, plaintext, len);
//    size_t padded_len = pad_pkcs7(buffer, len);
//
//    AES_init_ctx_iv(&ctx, aes_key, aes_iv);
//    AES_CBC_encrypt_buffer(&ctx, buffer, padded_len);
//
////    // Construir frame Ethernet con formato definido
////    for (int i = 0; i < 6; i++) {
////        g_frame[i] = 0x10;  // Dirección MAC destino (ejemplo fijo)
////    }
//        g_frame[0] = 0x10U;
//        g_frame[1] = 0x63U;
//        g_frame[2] = 0xC8U;
//        g_frame[3] = 0xF7U;
//        g_frame[4] = 0x6AU;
//        g_frame[5] = 0x63U;
//
//    memcpy(&g_frame[6], &g_macAddr[0], 6);  // MAC origen
//
//    // Longitud en LITTLE-ENDIAN
//    g_frame[12] = padded_len & 0xFF;
//    g_frame[13] = (padded_len >> 8) & 0xFF;
//
//    // Copiar el mensaje cifrado
//    memcpy(&g_frame[14], buffer, padded_len);
//
//    if (kStatus_Success == ENET_SendFrame(EXAMPLE_ENET, &g_handle, &g_frame[0], padded_len + 14, 0, false, NULL)) {
//        PRINTF("Encrypted frame sent!\r\n");
//
//        // Delay para adaptador USB-Ethernet
//        for (volatile int i = 0; i < 10000000; i++) {
//            asm("nop");
//        }
//    } else {
//        PRINTF("Failed to send encrypted frame!\r\n");
//    }
//}
