/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

/*!
 * @addtogroup pin_mux
 * @{
 */

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);

#define IOPCTL_PIO_ANAMUX_DI 0x00u        /*!<@brief Analog mux is disabled */
#define IOPCTL_PIO_FULLDRIVE_DI 0x00u     /*!<@brief Normal drive */
#define IOPCTL_PIO_FULLDRIVE_EN 0x0100u   /*!<@brief Full drive enable */
#define IOPCTL_PIO_FUNC0 0x00u            /*!<@brief Selects pin function 0 */
#define IOPCTL_PIO_FUNC1 0x01u            /*!<@brief Selects pin function 1 */
#define IOPCTL_PIO_FUNC5 0x05u            /*!<@brief Selects pin function 5 */
#define IOPCTL_PIO_INBUF_DI 0x00u         /*!<@brief Disable input buffer function */
#define IOPCTL_PIO_INBUF_EN 0x40u         /*!<@brief Enables input buffer function */
#define IOPCTL_PIO_INV_DI 0x00u           /*!<@brief Input function is not inverted */
#define IOPCTL_PIO_PSEDRAIN_DI 0x00u      /*!<@brief Pseudo Output Drain is disabled */
#define IOPCTL_PIO_PULLDOWN_EN 0x00u      /*!<@brief Enable pull-down function */
#define IOPCTL_PIO_PULLUP_EN 0x20u        /*!<@brief Enable pull-up function */
#define IOPCTL_PIO_PUPD_DI 0x00u          /*!<@brief Disable pull-up / pull-down function */
#define IOPCTL_PIO_PUPD_EN 0x10u          /*!<@brief Enable pull-up / pull-down function */
#define IOPCTL_PIO_SLEW_RATE_NORMAL 0x00u /*!<@brief Normal mode */

/*! @name FC0_RXD_SDA_MOSI_DATA (coord H16), J45[22]/U9[13]/U105[4]
  @{ */
/* Routed pin properties */
#define BOARD_INITPINS_DEBUG_UART_RX_PERIPHERAL FLEXCOMM0           /*!<@brief Peripheral name */
#define BOARD_INITPINS_DEBUG_UART_RX_SIGNAL RXD_SDA_MOSI_DATA       /*!<@brief Signal name */
                                                                    /* @} */

/*! @name FC0_TXD_SCL_MISO_WS (coord G16), J45[32]/U105[3]/U9[12]
  @{ */
/* Routed pin properties */
#define BOARD_INITPINS_DEBUG_UART_TXD_PERIPHERAL FLEXCOMM0           /*!<@brief Peripheral name */
#define BOARD_INITPINS_DEBUG_UART_TXD_SIGNAL TXD_SCL_MISO_WS         /*!<@brief Signal name */
                                                                     /* @} */

/*! @name SD0_CARD_DET_N (coord T3), J32[10]/SD0_CARD_DET_N
  @{ */
/* Routed pin properties */
#define BOARD_INITPINS_SD0_CARD_DET_N_PERIPHERAL USDHC0              /*!<@brief Peripheral name */
#define BOARD_INITPINS_SD0_CARD_DET_N_SIGNAL USDHC_CARD_DET          /*!<@brief Signal name */
                                                                     /* @} */

/*! @name SD0_D0 (coord U4), U111A[A3]/SD0_D0
  @{ */
/* Routed pin properties */
#define BOARD_INITPINS_SD0_D0_PERIPHERAL USDHC0        /*!<@brief Peripheral name */
#define BOARD_INITPINS_SD0_D0_SIGNAL USDHC_DATA        /*!<@brief Signal name */
#define BOARD_INITPINS_SD0_D0_CHANNEL 0                /*!<@brief Signal channel */
                                                       /* @} */

/*! @name SD0_D1 (coord T4), U111A[A4]/SD0_D1
  @{ */
/* Routed pin properties */
#define BOARD_INITPINS_SD0_D1_PERIPHERAL USDHC0        /*!<@brief Peripheral name */
#define BOARD_INITPINS_SD0_D1_SIGNAL USDHC_DATA        /*!<@brief Signal name */
#define BOARD_INITPINS_SD0_D1_CHANNEL 1                /*!<@brief Signal channel */
                                                       /* @} */

/*! @name SD0_D2 (coord T7), U111A[A5]/SD0_D2
  @{ */
/* Routed pin properties */
#define BOARD_INITPINS_SD0_D2_PERIPHERAL USDHC0        /*!<@brief Peripheral name */
#define BOARD_INITPINS_SD0_D2_SIGNAL USDHC_DATA        /*!<@brief Signal name */
#define BOARD_INITPINS_SD0_D2_CHANNEL 2                /*!<@brief Signal channel */
                                                       /* @} */

/*! @name SD0_D3 (coord U6), U111A[B2]/SD0_D3
  @{ */
/* Routed pin properties */
#define BOARD_INITPINS_SD0_D3_PERIPHERAL USDHC0        /*!<@brief Peripheral name */
#define BOARD_INITPINS_SD0_D3_SIGNAL USDHC_DATA        /*!<@brief Signal name */
#define BOARD_INITPINS_SD0_D3_CHANNEL 3                /*!<@brief Signal channel */
                                                       /* @} */

/*! @name SD0_CMD (coord R6), U111A[M5]/SD0_CMD
  @{ */
/* Routed pin properties */
#define BOARD_INITPINS_SD0_CMD_PERIPHERAL USDHC0         /*!<@brief Peripheral name */
#define BOARD_INITPINS_SD0_CMD_SIGNAL USDHC_CMD          /*!<@brief Signal name */
                                                         /* @} */

/*! @name SD0_CLK (coord R5), U111A[M6]/SD0_CLK
  @{ */
/* Routed pin properties */
#define BOARD_INITPINS_SD0_CLK_PERIPHERAL USDHC0         /*!<@brief Peripheral name */
#define BOARD_INITPINS_SD0_CLK_SIGNAL USDHC_CLK          /*!<@brief Signal name */
                                                         /* @} */

/*! @name SD0_DS (coord P6), U111A[H5]/SD0_WR_PRT_DS
  @{ */
/* Routed pin properties */
#define BOARD_INITPINS_SD0_WR_PRT_DS_PERIPHERAL USDHC0               /*!<@brief Peripheral name */
#define BOARD_INITPINS_SD0_WR_PRT_DS_SIGNAL USDHC_STROBE             /*!<@brief Signal name */
                                                                     /* @} */

/*! @name PIO2_10 (coord N5), SD0_RESET_N
  @{ */
/* Routed pin properties */
#define BOARD_INITPINS_SD0_RESET_N_PERIPHERAL GPIO           /*!<@brief Peripheral name */
#define BOARD_INITPINS_SD0_RESET_N_SIGNAL PIO2               /*!<@brief Signal name */
#define BOARD_INITPINS_SD0_RESET_N_CHANNEL 10                /*!<@brief Signal channel */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_SD0_RESET_N_GPIO GPIO                 /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_SD0_RESET_N_GPIO_PIN_MASK (1U << 10U) /*!<@brief GPIO pin mask */
#define BOARD_INITPINS_SD0_RESET_N_PORT 2U                   /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_SD0_RESET_N_PIN 10U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_SD0_RESET_N_PIN_MASK (1U << 10U)      /*!<@brief PORT pin mask */
                                                             /* @} */

/*! @name SD0_D4 (coord P5), U111A[B3]/SD0_D4
  @{ */
/* Routed pin properties */
#define BOARD_INITPINS_SD0_D4_PERIPHERAL USDHC0        /*!<@brief Peripheral name */
#define BOARD_INITPINS_SD0_D4_SIGNAL USDHC_DATA        /*!<@brief Signal name */
#define BOARD_INITPINS_SD0_D4_CHANNEL 4                /*!<@brief Signal channel */
                                                       /* @} */

/*! @name SD0_D5 (coord R4), U111A[B4]/SD0_D5
  @{ */
/* Routed pin properties */
#define BOARD_INITPINS_SD0_D5_PERIPHERAL USDHC0        /*!<@brief Peripheral name */
#define BOARD_INITPINS_SD0_D5_SIGNAL USDHC_DATA        /*!<@brief Signal name */
#define BOARD_INITPINS_SD0_D5_CHANNEL 5                /*!<@brief Signal channel */
                                                       /* @} */

/*! @name SD0_D6 (coord P4), U111A[B5]/SD0_D6
  @{ */
/* Routed pin properties */
#define BOARD_INITPINS_SD0_D6_PERIPHERAL USDHC0        /*!<@brief Peripheral name */
#define BOARD_INITPINS_SD0_D6_SIGNAL USDHC_DATA        /*!<@brief Signal name */
#define BOARD_INITPINS_SD0_D6_CHANNEL 6                /*!<@brief Signal channel */
                                                       /* @} */

/*! @name SD0_D7 (coord T6), U111A[B6]/SD0_D7
  @{ */
/* Routed pin properties */
#define BOARD_INITPINS_SD0_D7_PERIPHERAL USDHC0        /*!<@brief Peripheral name */
#define BOARD_INITPINS_SD0_D7_SIGNAL USDHC_DATA        /*!<@brief Signal name */
#define BOARD_INITPINS_SD0_D7_CHANNEL 7                /*!<@brief Signal channel */
                                                       /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void); /* Function assigned for the Cortex-M33 */

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
