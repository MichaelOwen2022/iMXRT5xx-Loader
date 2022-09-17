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
#define IOPCTL_PIO_FUNC0 0x00u            /*!<@brief Selects pin function 0 */
#define IOPCTL_PIO_FUNC1 0x01u            /*!<@brief Selects pin function 1 */
#define IOPCTL_PIO_INBUF_DI 0x00u         /*!<@brief Disable input buffer function */
#define IOPCTL_PIO_INBUF_EN 0x40u         /*!<@brief Enables input buffer function */
#define IOPCTL_PIO_INV_DI 0x00u           /*!<@brief Input function is not inverted */
#define IOPCTL_PIO_PSEDRAIN_DI 0x00u      /*!<@brief Pseudo Output Drain is disabled */
#define IOPCTL_PIO_PULLDOWN_EN 0x00u      /*!<@brief Enable pull-down function */
#define IOPCTL_PIO_PUPD_DI 0x00u          /*!<@brief Disable pull-up / pull-down function */
#define IOPCTL_PIO_SLEW_RATE_NORMAL 0x00u /*!<@brief Normal mode */

/*! @name FC0_TXD_SCL_MISO_WS (coord G16), J45[32]/U105[3]/U9[12]
  @{ */
/* Routed pin properties */
#define BOARD_INITPINS_DEBUG_UART_TXD_PERIPHERAL FLEXCOMM0           /*!<@brief Peripheral name */
#define BOARD_INITPINS_DEBUG_UART_TXD_SIGNAL TXD_SCL_MISO_WS         /*!<@brief Signal name */
                                                                     /* @} */

/*! @name FC0_RXD_SDA_MOSI_DATA (coord H16), J45[22]/U9[13]/U105[4]
  @{ */
/* Routed pin properties */
#define BOARD_INITPINS_DEBUG_UART_RX_PERIPHERAL FLEXCOMM0           /*!<@brief Peripheral name */
#define BOARD_INITPINS_DEBUG_UART_RX_SIGNAL RXD_SDA_MOSI_DATA       /*!<@brief Signal name */
                                                                    /* @} */

/*! @name PIO0_10 (coord K16), USER_KEY2
  @{ */
/* Routed pin properties */
#define BOARD_INITPINS_SW2_PERIPHERAL GPIO           /*!<@brief Peripheral name */
#define BOARD_INITPINS_SW2_SIGNAL PIO0               /*!<@brief Signal name */
#define BOARD_INITPINS_SW2_CHANNEL 10                /*!<@brief Signal channel */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_SW2_GPIO GPIO                 /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_SW2_GPIO_PIN_MASK (1U << 10U) /*!<@brief GPIO pin mask */
#define BOARD_INITPINS_SW2_PORT 0U                   /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_SW2_PIN 10U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_SW2_PIN_MASK (1U << 10U)      /*!<@brief PORT pin mask */
                                                     /* @} */

/*! @name PIO0_14 (coord B12), Q3[2]/PWM_RED_LED
  @{ */
/* Routed pin properties */
#define BOARD_INITPINS_LED_RED_PERIPHERAL GPIO           /*!<@brief Peripheral name */
#define BOARD_INITPINS_LED_RED_SIGNAL PIO0               /*!<@brief Signal name */
#define BOARD_INITPINS_LED_RED_CHANNEL 14                /*!<@brief Signal channel */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_LED_RED_GPIO GPIO                 /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_LED_RED_GPIO_PIN_MASK (1U << 14U) /*!<@brief GPIO pin mask */
#define BOARD_INITPINS_LED_RED_PORT 0U                   /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_LED_RED_PIN 14U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_LED_RED_PIN_MASK (1U << 14U)      /*!<@brief PORT pin mask */
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
