
//enet.h:
#ifndef ENET_H_
#define ENET_H_

#include <stdlib.h>
#include "pin_mux.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_enet.h"
#include "fsl_phy.h"
#include "fsl_enet_mdio.h"
#include "fsl_phyksz8081.h"
#include "fsl_sysmpu.h"


/* ENET base address */
#define EXAMPLE_ENET        ENET
#define EXAMPLE_PHY_ADDRESS 0x00U

/* MDIO operations. */
#define EXAMPLE_MDIO_OPS enet_ops
/* PHY operations. */
#define EXAMPLE_PHY_OPS phyksz8081_ops
/* ENET clock frequency. */
#define EXAMPLE_CLOCK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)
#define ENET_RXBD_NUM          (4)
#define ENET_TXBD_NUM          (4)
#define ENET_RXBUFF_SIZE       (ENET_FRAME_MAX_FRAMELEN)
#define ENET_TXBUFF_SIZE       (ENET_FRAME_MAX_FRAMELEN)
#define ENET_DATA_LENGTH       (1000)
#define ENET_TRANSMIT_DATA_NUM (20)
#define APP_ENET_BUFF_ALIGNMENT ENET_BUFF_ALIGNMENT
#define PHY_AUTONEGO_TIMEOUT_COUNT (100000)
#define PHY_STABILITY_DELAY_US (0U)
#define MAC_ADDRESS {0xd4, 0xbe, 0xd9, 0x45, 0x22, 0x60}

extern enet_rx_bd_struct_t g_rxBuffDescrip[ENET_RXBD_NUM];
extern enet_tx_bd_struct_t g_txBuffDescrip[ENET_TXBD_NUM];
extern uint8_t g_rxDataBuff[ENET_RXBD_NUM][SDK_SIZEALIGN(ENET_RXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT)];
extern uint8_t g_txDataBuff[ENET_TXBD_NUM][SDK_SIZEALIGN(ENET_TXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT)];
extern enet_handle_t g_handle;
extern uint8_t g_frame[ENET_DATA_LENGTH + 14];

/*! @brief The MAC address for ENET device. */
extern uint8_t g_macAddr[6];

/*! @brief Enet PHY and MDIO interface handler. */
extern mdio_handle_t mdioHandle;
extern phy_handle_t phyHandle;
extern uint32_t length;
extern status_t status;
extern enet_data_error_stats_t eErrStatic;
extern uint32_t testTxNum;
extern bool link;

extern     /* Prepare the buffer configuration. */
enet_buffer_config_t buffConfig[];
void enet_init(void);

#endif /* ENET_H_ */
