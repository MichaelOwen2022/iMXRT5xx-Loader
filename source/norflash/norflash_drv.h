#ifndef __NORFLASH_DRV_H__
#define __NORFLASH_DRV_H__

#include "norflash.h"

#if defined(CONFIG_DRV_FLEXSPI)
    #include "fsl_flexspi.h"

    int flexspi_init(void);
    int flexspi_command(uint32_t addr, uint8_t seqid);
    int flexspi_read(uint32_t addr, uint8_t seqid, uint32_t *data, size_t size);
    int flexspi_write(uint32_t addr, uint8_t seqid, uint32_t *data, size_t size);

    int flexspi_read_status(uint8_t *status);
    int flexspi_write_enable(void);
    int flexspi_erase_sector(uint32_t addr);
    int flexspi_read_page(uint32_t addr, uint8_t *data, size_t size);
    int flexspi_write_page(uint32_t addr, uint8_t *data, size_t size);
#else
    #error "Unknown norflash driver"
#endif

#endif/*__NORFLASH_DRV_H__*/
