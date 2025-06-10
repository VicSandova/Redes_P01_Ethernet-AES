// enet_hal.c
#include "enet_txrx_transfer.h"
#include "enet.h"     // define EXAMPLE_ENET, g_handle, g_frame, etc.

void ETHERNET_TxRx_Init(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    enet_init();
    bool link = false;
    while (!link)
        PHY_GetLinkStatus(&phyHandle, &link);
}

bool ETHERNET_Tx(uint8_t *frame, size_t length)
{
    return (ENET_SendFrame(EXAMPLE_ENET, &g_handle, frame, length, 0, false, NULL)
            == kStatus_Success);
}

size_t ETHERNET_PeekFrameSize(void)
{
    uint32_t len = 0;
    if (ENET_GetRxFrameSize(&g_handle, &len, 0) == kStatus_Success)
        return len;
    return 0;
}

bool ETHERNET_Rx(uint8_t *frame, size_t length)
{
    return (ENET_ReadFrame(EXAMPLE_ENET, &g_handle, frame, length, 0, NULL) == kStatus_Success);
}
