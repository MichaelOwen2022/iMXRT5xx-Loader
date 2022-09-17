/*
 * Copyright 2020-2021 NXP
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
#define IOPCTL_PIO_PSEDRAIN_EN 0x0400u    /*!<@brief Pseudo Output Drain is enabled */
#define IOPCTL_PIO_PULLDOWN_EN 0x00u      /*!<@brief Enable pull-down function */
#define IOPCTL_PIO_PUPD_DI 0x00u          /*!<@brief Disable pull-up / pull-down function */
#define IOPCTL_PIO_SLEW_RATE_NORMAL 0x00u /*!<@brief Normal mode */

/*! @name PIO4_5 (coord P16), RESET_OSPI_MEM
  @{ */
/* Routed pin properties */
#define BOARD_INITPINS_RESET_OSPI_MEM_PERIPHERAL GPIO          /*!<@brief Peripheral name */
#define BOARD_INITPINS_RESET_OSPI_MEM_SIGNAL PIO4              /*!<@brief Signal name */
#define BOARD_INITPINS_RESET_OSPI_MEM_CHANNEL 5                /*!<@brief Signal channel */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_RESET_OSPI_MEM_GPIO GPIO                /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_RESET_OSPI_MEM_GPIO_PIN_MASK (1U << 5U) /*!<@brief GPIO pin mask */
#define BOARD_INITPINS_RESET_OSPI_MEM_PORT 4U                  /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_RESET_OSPI_MEM_PIN 5U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_RESET_OSPI_MEM_PIN_MASK (1U << 5U)      /*!<@brief PORT pin mask */
                                                               /* @} */

/*! @name PIO0_14 (coord B12), LED_RED
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

/*! @name PIO1_0 (coord A10), LED_GREEN
  @{ */
/* Routed pin properties */
#define BOARD_INITPINS_LED_GREEN_PERIPHERAL GPIO          /*!<@brief Peripheral name */
#define BOARD_INITPINS_LED_GREEN_SIGNAL PIO1              /*!<@brief Signal name */
#define BOARD_INITPINS_LED_GREEN_CHANNEL 0                /*!<@brief Signal channel */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_LED_GREEN_GPIO GPIO                /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_LED_GREEN_GPIO_PIN_MASK (1U << 0U) /*!<@brief GPIO pin mask */
#define BOARD_INITPINS_LED_GREEN_PORT 1U                  /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_LED_GREEN_PIN 0U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_LED_GREEN_PIN_MASK (1U << 0U)      /*!<@brief PORT pin mask */
                                                          /* @} */

/*! @name PIO3_17 (coord D8), LED_BLUE
  @{ */
/* Routed pin properties */
#define BOARD_INITPINS_LED_BLUE_PERIPHERAL GPIO           /*!<@brief Peripheral name */
#define BOARD_INITPINS_LED_BLUE_SIGNAL PIO3               /*!<@brief Signal name */
#define BOARD_INITPINS_LED_BLUE_CHANNEL 17                /*!<@brief Signal channel */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_LED_BLUE_GPIO GPIO                 /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_LED_BLUE_GPIO_PIN_MASK (1U << 17U) /*!<@brief GPIO pin mask */
#define BOARD_INITPINS_LED_BLUE_PORT 3U                   /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_LED_BLUE_PIN 17U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_LED_BLUE_PIN_MASK (1U << 17U)      /*!<@brief PORT pin mask */
                                                          /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void); /* Function assigned for the Cortex-M33 */

#define IOPCTL_PIO_ANAMUX_DI 0x00u        /*!<@brief Analog mux is disabled */
#define IOPCTL_PIO_FULLDRIVE_DI 0x00u     /*!<@brief Normal drive */
#define IOPCTL_PIO_FUNC6 0x06u            /*!<@brief Selects pin function 6 */
#define IOPCTL_PIO_INBUF_DI 0x00u         /*!<@brief Disable input buffer function */
#define IOPCTL_PIO_INBUF_EN 0x40u         /*!<@brief Enables input buffer function */
#define IOPCTL_PIO_INV_DI 0x00u           /*!<@brief Input function is not inverted */
#define IOPCTL_PIO_PSEDRAIN_DI 0x00u      /*!<@brief Pseudo Output Drain is disabled */
#define IOPCTL_PIO_PULLDOWN_EN 0x00u      /*!<@brief Enable pull-down function */
#define IOPCTL_PIO_PUPD_DI 0x00u          /*!<@brief Disable pull-up / pull-down function */
#define IOPCTL_PIO_SLEW_RATE_NORMAL 0x00u /*!<@brief Normal mode */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitDEBUG_UARTPins(void); /* Function assigned for the Cortex-M33 */

#define IOPCTL_PIO_ANAMUX_DI 0x00u        /*!<@brief Analog mux is disabled */
#define IOPCTL_PIO_FULLDRIVE_DI 0x00u     /*!<@brief Normal drive */
#define IOPCTL_PIO_FUNC0 0x00u            /*!<@brief Selects pin function 0 */
#define IOPCTL_PIO_FUNC1 0x01u            /*!<@brief Selects pin function 1 */
#define IOPCTL_PIO_INBUF_DI 0x00u         /*!<@brief Disable input buffer function */
#define IOPCTL_PIO_INBUF_EN 0x40u         /*!<@brief Enables input buffer function */
#define IOPCTL_PIO_INV_DI 0x00u           /*!<@brief Input function is not inverted */
#define IOPCTL_PIO_PSEDRAIN_DI 0x00u      /*!<@brief Pseudo Output Drain is disabled */
#define IOPCTL_PIO_PULLDOWN_EN 0x00u      /*!<@brief Enable pull-down function */
#define IOPCTL_PIO_PULLUP_EN 0x20u        /*!<@brief Enable pull-up function */
#define IOPCTL_PIO_PUPD_DI 0x00u          /*!<@brief Disable pull-up / pull-down function */
#define IOPCTL_PIO_PUPD_EN 0x10u          /*!<@brief Enable pull-up / pull-down function */
#define IOPCTL_PIO_SLEW_RATE_NORMAL 0x00u /*!<@brief Normal mode */

/*! @name PIO4_0 (coord N15), SDC_3V3
  @{ */
/* Routed pin properties */
#define BOARD_INITPINSSD_SDC_3V3_PERIPHERAL GPIO                    /*!<@brief Peripheral name */
#define BOARD_INITPINSSD_SDC_3V3_SIGNAL PIO4                        /*!<@brief Signal name */
#define BOARD_INITPINSSD_SDC_3V3_CHANNEL 0                          /*!<@brief Signal channel */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINSSD_SDC_3V3_GPIO GPIO                          /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINSSD_SDC_3V3_GPIO_PIN_MASK (1U << 0U)           /*!<@brief GPIO pin mask */
#define BOARD_INITPINSSD_SDC_3V3_PORT 4U                            /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINSSD_SDC_3V3_PIN 0U                             /*!<@brief PORT pin number */
#define BOARD_INITPINSSD_SDC_3V3_PIN_MASK (1U << 0U)                /*!<@brief PORT pin mask */
                                                                    /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPinsSD(void); /* Function assigned for the Cortex-M33 */

#define IOPCTL_PIO_ANAMUX_DI 0x00u        /*!<@brief Analog mux is disabled */
#define IOPCTL_PIO_FULLDRIVE_DI 0x00u     /*!<@brief Normal drive */
#define IOPCTL_PIO_FUNC0 0x00u            /*!<@brief Selects pin function 0 */
#define IOPCTL_PIO_FUNC1 0x01u            /*!<@brief Selects pin function 1 */
#define IOPCTL_PIO_FUNC7 0x07u            /*!<@brief Selects pin function 7 */
#define IOPCTL_PIO_INBUF_DI 0x00u         /*!<@brief Disable input buffer function */
#define IOPCTL_PIO_INBUF_EN 0x40u         /*!<@brief Enables input buffer function */
#define IOPCTL_PIO_INV_DI 0x00u           /*!<@brief Input function is not inverted */
#define IOPCTL_PIO_PSEDRAIN_DI 0x00u      /*!<@brief Pseudo Output Drain is disabled */
#define IOPCTL_PIO_PULLDOWN_EN 0x00u      /*!<@brief Enable pull-down function */
#define IOPCTL_PIO_PULLUP_EN 0x20u        /*!<@brief Enable pull-up function */
#define IOPCTL_PIO_PUPD_DI 0x00u          /*!<@brief Disable pull-up / pull-down function */
#define IOPCTL_PIO_PUPD_EN 0x10u          /*!<@brief Enable pull-up / pull-down function */
#define IOPCTL_PIO_SLEW_RATE_NORMAL 0x00u /*!<@brief Normal mode */

/*! @name PIO3_20 (coord D6), SDIO_RST
  @{ */
/* Routed pin properties */
#define BOARD_INITPINSM2_SDIO_RST_PERIPHERAL GPIO                    /*!<@brief Peripheral name */
#define BOARD_INITPINSM2_SDIO_RST_SIGNAL PIO3                        /*!<@brief Signal name */
#define BOARD_INITPINSM2_SDIO_RST_CHANNEL 20                         /*!<@brief Signal channel */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINSM2_SDIO_RST_GPIO GPIO                          /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINSM2_SDIO_RST_GPIO_PIN_MASK (1U << 20U)          /*!<@brief GPIO pin mask */
#define BOARD_INITPINSM2_SDIO_RST_PORT 3U                            /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINSM2_SDIO_RST_PIN 20U                            /*!<@brief PORT pin number */
#define BOARD_INITPINSM2_SDIO_RST_PIN_MASK (1U << 20U)               /*!<@brief PORT pin mask */
                                                                     /* @} */

/*! @name PIO0_31 (coord D12), WL_RST
  @{ */
/* Routed pin properties */
#define BOARD_INITPINSM2_WL_RST_PERIPHERAL GPIO                    /*!<@brief Peripheral name */
#define BOARD_INITPINSM2_WL_RST_SIGNAL PIO0                        /*!<@brief Signal name */
#define BOARD_INITPINSM2_WL_RST_CHANNEL 31                         /*!<@brief Signal channel */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINSM2_WL_RST_GPIO GPIO                          /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINSM2_WL_RST_GPIO_PIN_MASK (1U << 31U)          /*!<@brief GPIO pin mask */
#define BOARD_INITPINSM2_WL_RST_PORT 0U                            /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINSM2_WL_RST_PIN 31U                            /*!<@brief PORT pin number */
#define BOARD_INITPINSM2_WL_RST_PIN_MASK (1U << 31U)               /*!<@brief PORT pin mask */
                                                                   /* @} */

/*! @name PIO4_3 (coord M14), M2_3V3
  @{ */
/* Routed pin properties */
#define BOARD_INITPINSM2_M2_3V3_PERIPHERAL GPIO                    /*!<@brief Peripheral name */
#define BOARD_INITPINSM2_M2_3V3_SIGNAL PIO4                        /*!<@brief Signal name */
#define BOARD_INITPINSM2_M2_3V3_CHANNEL 3                          /*!<@brief Signal channel */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINSM2_M2_3V3_GPIO GPIO                          /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINSM2_M2_3V3_GPIO_PIN_MASK (1U << 3U)           /*!<@brief GPIO pin mask */
#define BOARD_INITPINSM2_M2_3V3_PORT 4U                            /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINSM2_M2_3V3_PIN 3U                             /*!<@brief PORT pin number */
#define BOARD_INITPINSM2_M2_3V3_PIN_MASK (1U << 3U)                /*!<@brief PORT pin mask */
                                                                   /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPinsM2(void); /* Function assigned for the Cortex-M33 */

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
