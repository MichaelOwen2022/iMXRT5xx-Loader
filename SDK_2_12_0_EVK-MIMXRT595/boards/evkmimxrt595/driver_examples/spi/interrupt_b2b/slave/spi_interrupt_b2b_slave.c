/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_spi.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifndef USE_HS_SPI
#define USE_HS_SPI 0
#endif

#if USE_HS_SPI
#define EXAMPLE_SPI_SLAVE     SPI14
#define EXAMPLE_SPI_SLAVE_IRQ FLEXCOMM14_IRQn
#define SPI_SLAVE_IRQHandler  FLEXCOMM14_IRQHandler
#else
#define EXAMPLE_SPI_SLAVE     SPI5
#define EXAMPLE_SPI_SLAVE_IRQ FLEXCOMM5_IRQn
#define SPI_SLAVE_IRQHandler  FLEXCOMM5_IRQHandler
#endif

#define EXAMPLE_SPI_SSEL 0
#define EXAMPLE_SPI_SPOL kSPI_SpolActiveAllLow

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
#define BUFFER_SIZE (64)
static uint8_t srcBuff[BUFFER_SIZE];
static uint8_t destBuff[BUFFER_SIZE];
static uint32_t txIndex            = BUFFER_SIZE;
static uint32_t rxIndex            = BUFFER_SIZE;
static volatile bool slaveFinished = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

void SPI_SLAVE_IRQHandler(void)
{
    if ((SPI_GetStatusFlags(EXAMPLE_SPI_SLAVE) & kSPI_RxNotEmptyFlag) && (rxIndex > 0U))
    {
        destBuff[BUFFER_SIZE - rxIndex] = SPI_ReadData(EXAMPLE_SPI_SLAVE);
        rxIndex--;
    }

    if ((SPI_GetStatusFlags(EXAMPLE_SPI_SLAVE) & kSPI_TxNotFullFlag) && (txIndex > 0U))
    {
        SPI_WriteData(EXAMPLE_SPI_SLAVE, (uint16_t)(srcBuff[BUFFER_SIZE - txIndex]), 0);
        txIndex--;
    }

    if ((rxIndex == 0U) && (txIndex == 0U))
    {
        slaveFinished = true;
        SPI_DisableInterrupts(EXAMPLE_SPI_SLAVE, kSPI_RxLvlIrq | kSPI_TxLvlIrq);
    }
    SDK_ISR_EXIT_BARRIER;
}

int main(void)
{
    spi_slave_config_t slaveConfig = {0};
    uint32_t i                     = 0U;
    uint32_t err                   = 0U;

    /* Init the boards */
#if USE_HS_SPI
    /* Use 48 MHz clock for the FLEXCOMM14 */
    CLOCK_AttachClk(kFRO_DIV4_to_FLEXCOMM14);
#else
    /* Use 48 MHz clock for the FLEXCOMM5 */
    CLOCK_AttachClk(kFRO_DIV4_to_FLEXCOMM5);
#endif

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("\r\nSPI board to board interrupt slave example started!\r\n");

    /* Init SPI slave */
    /*
     * slaveConfig->enableSlave = true;
     * slaveConfig->polarity = kSPI_ClockPolarityActiveHigh;
     * slaveConfig->phase = kSPI_ClockPhaseFirstEdge;
     * slaveConfig->direction = kSPI_MsbFirst;
     */
    SPI_SlaveGetDefaultConfig(&slaveConfig);
    slaveConfig.sselPol = (spi_spol_t)EXAMPLE_SPI_SPOL;
    SPI_SlaveInit(EXAMPLE_SPI_SLAVE, &slaveConfig);

    /* Init source buffer */
    for (i = 0U; i < BUFFER_SIZE; i++)
    {
        srcBuff[i] = i;
    }

    /* Enable interrupt, first enable slave and then master. */
    EnableIRQ(EXAMPLE_SPI_SLAVE_IRQ);
    SPI_EnableInterrupts(EXAMPLE_SPI_SLAVE, kSPI_RxLvlIrq | kSPI_TxLvlIrq);

    while (slaveFinished != true)
    {
    }

    /* Check the data received */
    for (i = 0U; i < BUFFER_SIZE; i++)
    {
        if (destBuff[i] != srcBuff[i])
        {
            PRINTF("\r\nThe %d data is wrong, the data received is %d \r\n", i, destBuff[i]);
            err++;
        }
    }
    if (err == 0U)
    {
        PRINTF("\r\nSPI transfer finished!\r\n");
    }

    while (1)
    {
    }
}
