
#include <stdio.h>
#include <stdarg.h>

#include "log.h"
#include "fsl_usart.h"

#define BOARD_DEBUG_UART_BASEADDR   (uint32_t)USART0
#define BOARD_DEBUG_UART_INSTANCE   0U
#define BOARD_DEBUG_UART_CLK_FREQ   CLOCK_GetFlexcommClkFreq(0)
#define BOARD_DEBUG_UART_FRG_CLK    (&(const clock_frg_clk_config_t){0U, kCLOCK_FrgPllDiv, 255U, 0U}) /*!< Select FRG0 mux as frg_pll */
#define BOARD_DEBUG_UART_CLK_ATTACH kFRG_to_FLEXCOMM0

int log_init(void)
{
    uint32_t uartClkSrcFreq;

    /* attach FRG0 clock to FLEXCOMM0 (debug console) */
    CLOCK_SetFRGClock(BOARD_DEBUG_UART_FRG_CLK);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    uartClkSrcFreq = BOARD_DEBUG_UART_CLK_FREQ;

    usart_config_t usartConfig;
	USART_GetDefaultConfig(&usartConfig);

    usartConfig.baudRate_Bps = 115200U;
    usartConfig.parityMode = kUSART_ParityDisabled;
    usartConfig.stopBitCount = kUSART_OneStopBit;
    USART_Init(USART0, &usartConfig, uartClkSrcFreq);

    log_printf("\r\n\x1b[2J\x1b[;H\r\n");
    log_printf("***********************************************************\r\n");
    log_printf("****************** MIMXRT595 FlashLoader ******************\r\n");
    log_printf("***********************************************************\r\n");

    return 0;
}

int log_printf(const char *fmt, ...)
{
    char buf[128];
    va_list args;
    int n = 0;

    va_start(args, fmt);
    n = vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    USART_WriteBlocking(USART0, (const uint8_t *)buf, n);

    return n;
}
