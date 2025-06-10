#include "secure_link.h"
#include "fsl_debug_console.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef void (*SecureApp_RxCb)(const uint8_t *data, size_t len);

static SecureApp_RxCb rxCb = NULL;

//const char *mensaje = "Aún hay esperanza...";
const char *mensaje = "No todo lo que es oro reluce...";

void SecureApp_Process(void)
{
    uint8_t buf[256];
    size_t len;
    if (SecureLink_Receive(buf, &len) && rxCb)	/* Funcion pedida como requerimiento: SecureLink_Receive */
    {
        rxCb(buf, len);
    }
}

static void app_rx_handler(const uint8_t *msg, size_t len)
{
    PRINTF(">> Mensaje App recibido (%d bytes): %s\r\n", len, msg);
}

int main(void)
{
	SecureLink_Init();	/* Funcion pedida como requerimiento SecureLink_Init */
	rxCb = app_rx_handler;


    while (1)
    {
    	SecureLink_Send((const uint8_t*)mensaje, strlen(mensaje));	/* Funcion pedida como requerimiento: SecureLink_Send Tx*/
        SecureApp_Process();//Rx

        for (volatile int i=0; i<10000000; i++) asm("nop");	// un pequeño delay
    }
}
