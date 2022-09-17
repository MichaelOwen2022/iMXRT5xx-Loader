
#include "watchdog_cfg.h"

#if defined(CONFIG_EXTERNAL_WATCHDOG)

#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "fsl_i2c.h"
#include "fsl_gpio.h"

/********************************************************************************************************************************/

#define PWR_PORT        2
#define PWR_PIN         28

#define PWR_AS_OUTPUT(lvl) \
({ \
    gpio_pin_config_t config; \
    config.pinDirection = kGPIO_DigitalOutput; \
    config.outputLogic  = (lvl); \
    GPIO_PinInit(GPIO, PWR_PORT, PWR_PIN, &config); \
})

#define I2C_CLOCK_CAT(x)    kFRO_DIV4_to_FLEXCOMM##x
#define I2C_BASE_CAT(x)     I2C##x

#define I2C_CLOCK   I2C_CLOCK_CAT(5)
#define I2C_BASE    I2C_BASE_CAT(5)
#define I2C_CLKSRC  CLOCK_GetFlexcommClkFreq(5)
#define I2C_FREQ    (100000)
#define I2C_ADDR    (0x0B)

static void software_i2c_init(void)
{
    i2c_master_config_t config;

    GPIO_PortInit(GPIO, PWR_PORT);
    PWR_AS_OUTPUT(1);
    SDK_DelayAtLeastUs(100000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    /* Use 48 MHz clock for the FLEXCOMMx */
    CLOCK_AttachClk(I2C_CLOCK);

    I2C_MasterGetDefaultConfig(&config);
    config.baudRate_Bps = I2C_FREQ;
    I2C_MasterInit(I2C_BASE, &config, I2C_CLKSRC);
}

static int i2c_read_reg(uint8_t reg, uint8_t *val)
{
    i2c_master_transfer_t masterXfer;

    /* Prepare transfer structure. */
    masterXfer.slaveAddress   = I2C_ADDR;
    masterXfer.direction      = kI2C_Read;
    masterXfer.subaddress     = reg;
    masterXfer.subaddressSize = 1;
    masterXfer.data           = val;
    masterXfer.dataSize       = 1;
    masterXfer.flags          = kI2C_TransferDefaultFlag;

    return I2C_MasterTransferBlocking(I2C_BASE, &masterXfer);
}

static int i2c_write_reg(uint8_t reg, uint8_t val)
{
    i2c_master_transfer_t masterXfer;

    /* Prepare transfer structure. */
    masterXfer.slaveAddress   = I2C_ADDR;
    masterXfer.direction      = kI2C_Write;
    masterXfer.subaddress     = reg;
    masterXfer.subaddressSize = 1;
    masterXfer.data           = &val;
    masterXfer.dataSize       = 1;
    masterXfer.flags          = kI2C_TransferDefaultFlag;

    return I2C_MasterTransferBlocking(I2C_BASE, &masterXfer);
}

/********************************************************************************************************************************/

int watchdog_external_init(void)
{
    int retval = 0;
    WDT_ENTER("");

    software_i2c_init();

    uint8_t version = 0;
    i2c_read_reg(0x00, &version);
    WDT_INFO("WDT: version[0x%02x]", version);

    WDT_LEAVE("%d", retval);
    return retval;
}

int watchdog_external_start(void)
{
    int retval = 0;
    WDT_ENTER("");

    i2c_write_reg(0x08, 0x93);

    WDT_LEAVE("%d", retval);
    return retval;
}

int watchdog_external_feed(void)
{
    int retval = 0;
    WDT_ENTER("");

    i2c_write_reg(0x08, 0x93);

    WDT_LEAVE("%d", retval);
    return retval;
}

int watchdog_external_stop(void)
{
    int retval = 0;
    WDT_ENTER("");

    i2c_write_reg(0x08, 0x13);

    WDT_LEAVE("%d", retval);
    return retval;
}

#endif
