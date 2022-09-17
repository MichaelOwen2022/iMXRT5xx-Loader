#ifndef __NORFLASH_LUT_H__
#define __NORFLASH_LUT_H__

#define LUT_SEQ_SIZE    (4)
#define LUT_MAX_SIZE    (64 / LUT_SEQ_SIZE)

typedef struct
{
    uint32_t table[LUT_SEQ_SIZE];
} lut_seq_t;

typedef enum
{
    LUT_SEQ_IDX_READ_STATUS,
    LUT_SEQ_IDX_WRITE_ENABLE,
    LUT_SEQ_IDX_ERASE_SECTOR,
    LUT_SEQ_IDX_READ_PAGE,
    LUT_SEQ_IDX_WRITE_PAGE,
    LUT_SEQ_IDX_USER_COMMAND,
    LUT_SEQ_IDX_USER_READ,
    LUT_SEQ_IDX_USER_WRITE,
    LUT_SEQ_IDX_MAXNBR
} lut_seq_idx_t;

#if (LUT_SEQ_IDX_MAXNBR >= LUT_MAX_SIZE)
#error "LUT_SEQ_IDX_MAXNBR is out of range !!!"
#endif

#endif/*__NORFLASH_LUT_H__*/
