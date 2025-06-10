#ifndef ENET_TXRX_TRANSFER_H_
#define ENET_TXRX_TRANSFER_H_

#include <stdint.h>
#include <stddef.h>
#include "fsl_debug_console.h"
#include "fsl_enet.h"
#include "fsl_phy.h"

void     ETHERNET_TxRx_Init(void);
bool     ETHERNET_Tx(uint8_t *frame, size_t length);
size_t   ETHERNET_PeekFrameSize(void);
bool     ETHERNET_Rx(uint8_t *frame, size_t length);

#endif /* ENET_TXRX_TRANSFER_H_ */
