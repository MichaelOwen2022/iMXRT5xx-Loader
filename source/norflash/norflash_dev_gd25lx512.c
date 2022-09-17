
#include "norflash_dev.h"
#include "norflash_drv.h"

#if defined(CONFIG_DEV_GD25LX512)

#include "log.h"
#include "norflash_lut.h"

static int gd25lx512_read_chipid(uint32_t *chipid, bool multiline)
{
    const lut_seq_t lut_seq[2] =
    {
        {
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x9F, kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 4)
        },
        {
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0x9F, kFLEXSPI_Command_DUMMY_DDR, kFLEXSPI_8PAD, 16),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_DDR, kFLEXSPI_8PAD, 4, 0, 0, 0),
        }
    };
    lut_seq_idx_t seqid = LUT_SEQ_IDX_USER_READ;
    int retval = 0;

    FLEXSPI_UpdateLUT(FLEXSPI_BASE, (seqid * LUT_SEQ_SIZE), (const uint32_t *)lut_seq[!!multiline].table, LUT_SEQ_SIZE);

    retval = flexspi_read(0, seqid, chipid, 4);
    if (retval == kStatus_Success)
    {
        DEV_INFO("%s(%p, %d) chipid[0x%08x]", __func__, chipid, multiline, *chipid);
    }

    return retval;
}

static int gd25lx512_read_status(uint8_t *status, bool multiline)
{
    const lut_seq_t lut_seq[2] =
    {
        {
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x05, kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 1),
        },
        {
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0x05, kFLEXSPI_Command_DUMMY_DDR, kFLEXSPI_8PAD, 16),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_DDR, kFLEXSPI_8PAD, 1, 0, 0, 0),
        }
    };
    lut_seq_idx_t seqid = LUT_SEQ_IDX_USER_READ;
    uint32_t _status = 0;
    int retval = 0;

    FLEXSPI_UpdateLUT(FLEXSPI_BASE, (seqid * LUT_SEQ_SIZE), (const uint32_t *)lut_seq[!!multiline].table, LUT_SEQ_SIZE);

    retval = flexspi_read(0, seqid, &_status, 1);
    if (retval == kStatus_Success)
    {
        *status = (uint8_t)_status;
        DEV_DEBUG("%s(%p, %d) status[0x%02x]", __func__, status, multiline, *status);
    }

    return retval;
}

static int gd25lx512_write_enable(bool multiline)
{
    const lut_seq_t lut_seq[2] =
    {
        {
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x06, 0, 0, 0),
        },
        {
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0x06, 0, 0, 0),
        }
    };
    lut_seq_idx_t seqid = LUT_SEQ_IDX_USER_COMMAND;
    int retval = 0;

    FLEXSPI_UpdateLUT(FLEXSPI_BASE, (seqid * LUT_SEQ_SIZE), (const uint32_t *)lut_seq[!!multiline].table, LUT_SEQ_SIZE);

    retval = flexspi_command(0, seqid);

    return retval;
}

static int gd25lx512_read_volatile(uint32_t reg, uint8_t *val, bool multiline)
{
    const lut_seq_t lut_seq[2] =
    {
        {
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x85, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 24),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_1PAD, 8, kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 1),
        },
        {
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0x85, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 32),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DUMMY_DDR, kFLEXSPI_8PAD, 16, kFLEXSPI_Command_READ_DDR, kFLEXSPI_8PAD, 1),
        }
    };
    lut_seq_idx_t seqid = LUT_SEQ_IDX_USER_READ;
    uint32_t data = 0;
    int retval = 0;

    FLEXSPI_UpdateLUT(FLEXSPI_BASE, (seqid * LUT_SEQ_SIZE), (const uint32_t *)lut_seq[!!multiline].table, LUT_SEQ_SIZE);

    retval = flexspi_read(reg, seqid, &data, 1);
    if (retval == kStatus_Success)
    {
        *val = (uint8_t)(data & 0xff);
        DEV_INFO("%s(0x%08x, %p, %d) val[0x%02x]", __func__, reg, val, multiline, *val);
    }

    return retval;
}

static int gd25lx512_write_volatile(uint32_t reg, uint8_t val, bool multiline)
{
    const lut_seq_t lut_seq[2] =
    {
        {
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x81, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 24),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 1, 0, 0, 0),
        },
        {
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0x81, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 32),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_DDR, kFLEXSPI_8PAD, 1, 0, 0, 0),
        }
    };
    lut_seq_idx_t seqid = LUT_SEQ_IDX_USER_WRITE;
    uint32_t data = 0;
    int retval = 0;

    FLEXSPI_UpdateLUT(FLEXSPI_BASE, (seqid * LUT_SEQ_SIZE), (const uint32_t *)lut_seq[!!multiline].table, LUT_SEQ_SIZE);

    data = (uint32_t)val;

    retval = flexspi_write(reg, seqid, &data, 1);
    if (retval == kStatus_Success)
    {
        DEV_INFO("%s(0x%08x, 0x%02x, %d)", __func__, reg, val, multiline);
    }

    return retval;
}

static void gd25lx512_update_lut(void)
{
    const lut_seq_t lut_seq[LUT_MAX_SIZE] =
    {
        [LUT_SEQ_IDX_READ_STATUS] =
        {
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0x05, kFLEXSPI_Command_DUMMY_DDR, kFLEXSPI_8PAD, 16),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_DDR, kFLEXSPI_8PAD, 1, 0, 0, 0),
        },
        [LUT_SEQ_IDX_WRITE_ENABLE] =
        {
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0x06, 0, 0, 0),
        },
        [LUT_SEQ_IDX_ERASE_SECTOR] =
        {
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0x20, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 32),
        },
        [LUT_SEQ_IDX_READ_PAGE] =
        {
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0x0B, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 32),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DUMMY_DDR, kFLEXSPI_8PAD, 32, kFLEXSPI_Command_READ_DDR, kFLEXSPI_8PAD, 4),
        },
        [LUT_SEQ_IDX_WRITE_PAGE] =
        {
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0x12, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 32),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_DDR, kFLEXSPI_8PAD, 4, 0, 0, 0),
        },
    };

    for (int i = 0; i < LUT_MAX_SIZE; i++)
    {
        DEV_DEBUG("LUT[%02d]: 0x%08x 0x%08x 0x%08x 0x%08x", i,
                    lut_seq[i].table[0], lut_seq[i].table[1], lut_seq[i].table[2], lut_seq[i].table[3]);
    }

    FLEXSPI_UpdateLUT(FLEXSPI_BASE, 0, (const uint32_t *)lut_seq, (LUT_MAX_SIZE * LUT_SEQ_SIZE));
}

int gd25lx512_probe(void)
{
    uint32_t chipid = 0; // 0xFF1A68C8
    int retval = 0;

    DEV_ENTER("");

    retval = gd25lx512_read_chipid(&chipid, false);
    if ((retval == 0) && (chipid == 0xFF1A68C8))
    { // Switch to DDR OPI
        uint8_t timeout = 20;

        gd25lx512_write_enable(false);
        gd25lx512_write_volatile(0x00, 0xe7, false);

        do {
            uint8_t status = 0;
            gd25lx512_read_status(&status, true);
            if ((status & 0x01) == 0)
                break;
        } while (--timeout > 0);

        if (timeout <= 0)
            retval = -1;
    }

    retval = gd25lx512_read_chipid(&chipid, true);
    if ((retval == 0) && (chipid == 0xFF1A68C8))
    {
        uint8_t value = 0;
        gd25lx512_read_volatile(0x00, &value, true);

        gd25lx512_update_lut();
    }
    else
    {
        retval = -1;
    }

    DEV_LEAVE("%d", retval);
    return retval;
}

#endif
