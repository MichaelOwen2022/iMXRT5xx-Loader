#ifndef __NORFLASH_H__
#define __NORFLASH_H__

#include "norflash_cfg.h"

int norflash_init(void);

int norflash_erase_sector(uint32_t addr);

int norflash_read_page(uint32_t addr, uint8_t *data, uint32_t size);

int norflash_write_page(uint32_t addr, uint8_t *data, uint32_t size);

#endif/*__NORFLASH_H__*/
