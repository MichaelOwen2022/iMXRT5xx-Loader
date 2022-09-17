#ifndef __NORFLASH_CONFIG_H__
#define __NORFLASH_CONFIG_H__

#include <stdint.h>
#include <stdbool.h>

#define B(s)    (s)
#define K(s)    ((s) * 1024)
#define M(s)    ((s) * 0x100000)

#ifndef MIN
#define MIN(x, y)   (((x) < (y)) ? (x) : (y))
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a)       (sizeof(a) / sizeof(a[0]))
#endif

/********************************************************************************************************************************/

#define FLEXSPI_BASE    (FLEXSPI0)
#define FLEXSPI_PORT    (kFLEXSPI_PortA1)

#define CONFIG_CAPACITY_64M

// #define CONFIG_READBACK_CHECK

#define CONFIG_DRV_FLEXSPI

#if defined(CONFIG_CAPACITY_64M)
    #define FLASH_CAPACITY      M(64)
    #define FLASH_BLOCK_SIZE    B(256)  // read / write size
    #define FLASH_ERASE_SIZE    K(4)    // erase sector size

    #define CONFIG_DEV_MX25UM512
    #define CONFIG_DEV_MX25UM512_OPI_MODE   1 // 0: SPI 1: STR-OPI, 2:DTR-OPI
    #define CONFIG_DEV_GD25LX512
#endif

#endif/*__NORFLASH_CONFIG_H__*/
