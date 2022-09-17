
#include "fsl_gpio.h"

#include "log.h"
#include "norflash_drv.h"
#include "norflash_dev.h"

/**********************************************************************************************************************/

typedef struct
{
    uint32_t port;
    uint32_t pin;
} gpio_pin_t;

static void norflash_reset(void)
{
    gpio_pin_config_t config;
    const gpio_pin_t rst_pins[] =
    {
        {4,  5}, // 兼容MIMXRT595-EVK开发板的复位脚
        {3, 28},
    };

    config.pinDirection = kGPIO_DigitalOutput;
    config.outputLogic  = 1;

    for (int i = 0; i < ARRAY_SIZE(rst_pins); i++)
    {
        GPIO_PortInit(GPIO, rst_pins[i].port);
        GPIO_PinInit(GPIO, rst_pins[i].port, rst_pins[i].pin, &config);

        GPIO_PinWrite(GPIO, rst_pins[i].port, rst_pins[i].pin, 1);
        SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

        GPIO_PinWrite(GPIO, rst_pins[i].port, rst_pins[i].pin, 0);
        SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

        GPIO_PinWrite(GPIO, rst_pins[i].port, rst_pins[i].pin, 1);
        SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

        NOR_INFO("Reset flash with port[%u] pin[%u]", rst_pins[i].port, rst_pins[i].pin);
    }
}

static uint32_t address_translate(uint32_t addr)
{
    uint32_t dest = 0;
    int i;

    const uint32_t start[] =
    {
        0x00000000, // Norflash Interface address.
        0x08000000, // FlexSPI0 Interface memory-mapped space, Non-Secure Start address.
        0x18000000  // FlexSPI0 Interface memory-mapped space, Secure Start address.
    };

    for (i = 0; i < ARRAY_SIZE(start); i++)
    {
        if ((addr >= start[i]) &&
            (addr < (start[i] + FLASH_CAPACITY)))
        {
            dest = addr - start[i];
            break;
        }
    }

    if (i >= ARRAY_SIZE(start))
    {
        NOR_ERROR("Invalid addr[0x%08x]", addr);
        dest = addr;
    }

    // NOR_DEBUG("%s(0x%08x -> 0x%08x)", __func__, addr, dest);

    return dest;
}

/**********************************************************************************************************************/

int norflash_init(void)
{
    int retval = 0;
    NOR_ENTER("");

#if defined(CONFIG_DRV_FLEXSPI)
    retval = flexspi_init();
#else
    retval = -1;
#endif
    if (retval == 0)
    {
        norflash_dev_ops_t ops[NORFLASH_DEV_MAXNBR] = {0};
        int devid = 0;

    #if defined(CONFIG_DEV_MX25UM512)
        ops[devid++].probe = mx25um512_probe;
    #endif
    #if defined(CONFIG_DEV_GD25LX512)
        ops[devid++].probe = gd25lx512_probe;
    #endif

        for (devid = 0; devid < NORFLASH_DEV_MAXNBR; devid++)
        {
            if (ops[devid].probe)
            {
                norflash_reset();

                if (ops[devid].probe() == 0)
                {
                    break;
                }
            }
        }

        if (devid >= NORFLASH_DEV_MAXNBR)
        {
            retval = -1;
        }
    }

    NOR_LEAVE("%d", retval);
    return retval;
}

int norflash_erase_sector(uint32_t addr)
{
    uint8_t status = 0;
    int retval = 0;

    addr = address_translate(addr);

    NOR_ENTER("0x%08x", addr);

    retval = flexspi_write_enable();
    if (retval == 0)
    {
        retval = flexspi_read_status(&status);

        retval = flexspi_erase_sector(addr);
        if (retval == 0)
        {
            do {
                retval = flexspi_read_status(&status);
                if (retval != 0)
                {
                    break;
                }
            } while (status & 0x01);
        }
    }

    NOR_LEAVE("%d", retval);
    return retval;
}

int norflash_read_page(uint32_t addr, uint8_t *data, uint32_t size)
{
    int retval = 0;

    addr = address_translate(addr);

    NOR_ENTER("0x%08x, %p, %u", addr, data, size);

    retval = flexspi_read_page(addr, data, size);

    NOR_LEAVE("%d", retval);
    return retval;
}

int norflash_write_page(uint32_t addr, uint8_t *data, uint32_t size)
{
    uint8_t status = 0;
    int retval = 0;

    addr = address_translate(addr);

    NOR_ENTER("0x%08x, %p, %u", addr, data, size);

    retval = flexspi_write_enable();
    if (retval == 0)
    {
        retval = flexspi_read_status(&status);

        retval = flexspi_write_page(addr, data, size);
        if (retval == 0)
        {
            do {
                retval = flexspi_read_status(&status);
                if (retval != 0)
                {
                    break;
                }
            } while (status & 0x01);
        }
    }

    NOR_LEAVE("%d", retval);
    return retval;
}

int norflash_exit(void)
{
    norflash_reset();

    return 0;
}
