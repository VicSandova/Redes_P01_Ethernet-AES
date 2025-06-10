// secure_link.c
#include "secure_link.h"
#include "enet_txrx_transfer.h"
#include "Aes_DR.h"
#include <string.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define AES_BLOCK_SIZE 16U  /*!< Tamaño de bloque AES en bytes */

/*! Clave AES de 16 bytes para cifrado CBC */
static const uint8_t key[16] = {
    'M','y','1','6','b','y','t','e','K','e','y','0','0','0','0','0'
};

/*! Vector de inicialización (IV) de 16 bytes para cifrado CBC */
static const uint8_t iv[16]  = {
    'M','y','1','6','b','y','t','e','I','V','0','0','0','0','0','0'
};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Aplica padding PKCS#7 a los datos para ajustar al tamaño de bloque AES.
 *
 * @param data Puntero al búfer con espacio reservado para el padding.
 * @param len  Longitud de los datos originales en bytes.
 * @return     Longitud total tras aplicar el padding.
 */
static size_t pad_pkcs7(uint8_t *data, size_t len);

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Inicializa la capa de enlace seguro.
 *
 * Llama a la inicialización de la interfaz Ethernet (HAL).
 */
void SecureLink_Init(void)
{
    ETHERNET_TxRx_Init();
}

/*!
 * @brief Cifra un payload y envía una trama Ethernet segura.
 *
 * @param plain Puntero al payload en claro.
 * @param len   Longitud del payload en bytes (debe estar entre 1 y 112).
 * @return      true si el envío fue exitoso; false en caso contrario.
 */
bool SecureLink_Send(const uint8_t *plain, size_t len)
{
    if (len == 0U || len > 112U) { return false; }	// No cumple con el tamaño

    /* Copiar datos y aplicar padding PKCS7 */
    static uint8_t buf[128];
    memcpy(buf, plain, len);
    size_t clen = pad_pkcs7(buf, len);

    /* Inicializar contexto AES-CBC y cifrar */
    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, key, iv);
    AES_CBC_encrypt_buffer(&ctx, buf, clen);

    /* Empaquetar trama: MAC destino / MAC origen / longitud / payload */
    extern uint8_t g_frame[];   /*!< Búfer de trama definido en enet_txrx_transfer.h */
    extern uint8_t g_macAddr[]; /*!< Dirección MAC origen */

    /* MAC destino fija */
    g_frame[0] = 0x00; g_frame[1] = 0x90;  g_frame[2] = 0x9E; g_frame[3] = 0x9A;  g_frame[4] = 0x9F ;g_frame[5] = 0x42;
    /* MAC origen */
    memcpy(&g_frame[6], g_macAddr, 6);
    /* Longitud del payload en little-endian */
    g_frame[12] = (uint8_t)(clen & 0xFF);
    g_frame[13] = (uint8_t)((clen >> 8) & 0xFF);
    /* Copiar payload cifrado */
    memcpy(&g_frame[14], buf, clen);

    /* Enviar trama completa */
    return ETHERNET_Tx(g_frame, clen + 14U);
}

/*!
 * @brief Recibe una trama Ethernet segura, la descifra y valida el padding.
 *
 * @param out     Búfer de salida para el payload descifrado.
 * @param out_len Puntero donde se deja la longitud del payload en bytes.
 * @return        true si la trama fue recibida y descifrada correctamente; false en caso contrario.
 */
bool SecureLink_Receive(uint8_t *out, size_t *out_len)
{
    /* Verificar longitud mínima de trama */
    size_t frame_len = ETHERNET_PeekFrameSize();
    if (frame_len < 14U)
    {
        return false;
    }

    /* Leer la trama completa */
    uint8_t frame[1514];
    if (!ETHERNET_Rx(frame, frame_len))
    {
        return false;
    }

    /* Extraer longitud del payload (bytes 12-13, little-endian) */
    uint16_t plen = (uint16_t)frame[12] | ((uint16_t)frame[13] << 8);
    if (plen == 0U || plen > frame_len - 14U || (plen % AES_BLOCK_SIZE) != 0U)
    {
        return false;
    }

    /* Copiar payload cifrado */
    static uint8_t buf[1500];
    memcpy(buf, &frame[14], plen);

    /* Inicializar contexto AES-CBC y descifrar */
    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, key, iv);
    AES_CBC_decrypt_buffer(&ctx, buf, plen);

    /* Validar padding PKCS 7 */
    uint8_t p = buf[plen - 1U];
    if (p == 0U || p > AES_BLOCK_SIZE)
    {
        return false;
    }
    for (uint8_t i = 0U; i < p; i++)
    {
        if (buf[plen - 1U - i] != p)
        {
            return false;
        }
    }

    /* Copiar payload descifrado y agregar terminador null */
    size_t msglen = plen - (size_t)p;
    memcpy(out, buf, msglen);
    out[msglen] = '\0';
    *out_len = msglen;

    return true;
}

/*!
 * @brief Aplica padding PKCS 7 a los datos.
 *
 * @param data Puntero al búfer con espacio para el padding.
 * @param len  Longitud de los datos originales en bytes.
 * @return     Longitud total tras padding.
 */
static size_t pad_pkcs7(uint8_t *data, size_t len)
{
    size_t p = AES_BLOCK_SIZE - (len % AES_BLOCK_SIZE);
    for (size_t i = 0U; i < p; i++)
    {
        data[len + i] = (uint8_t)p;
    }
    return len + p;
}

