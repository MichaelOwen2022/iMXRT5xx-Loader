#ifndef __NORFLASH_DEV_H__
#define __NORFLASH_DEV_H__

#include "norflash.h"

#if defined(CONFIG_DEV_MX25UM512)
    int mx25um512_probe(void);
#endif

#if defined(CONFIG_DEV_GD25LX512)
    int gd25lx512_probe(void);
#endif

typedef enum
{
#if defined(CONFIG_DEV_MX25UM512)
    NORFLASH_DEV_MX25UM512,
#endif
#if defined(CONFIG_DEV_GD25LX512)
    NORFLASH_DEV_GD25LX512,
#endif
    NORFLASH_DEV_MAXNBR
} norflash_dev_t;

typedef struct
{
    int (*probe)(void);
} norflash_dev_ops_t;

#endif/*__NORFLASH_DEV_H__*/
