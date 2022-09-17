
#include "norflash_dev.h"
#include "norflash_drv.h"

#if defined(CONFIG_DEV_MX25UM512)

#include "log.h"
#include "norflash_lut.h"

static int mx25um512_read_chipid(uint32_t *chipid, bool multiline)
{
    const lut_seq_t lut_seq[2] =
    {
        {
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x9F, kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 4)
        },
        {
        #if (CONFIG_DEV_MX25UM512_OPI_MODE == 1)
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0x9F, kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0x60),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_8PAD, 32, kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_8PAD, 4),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR, kFLEXSPI_8PAD, 4, 0, 0, 0),
        #else
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x9F, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x60),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 32, kFLEXSPI_Command_DUMMY_DDR, kFLEXSPI_8PAD, 8),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_DDR, kFLEXSPI_8PAD, 4, 0, 0, 0),
        #endif
        }
    };
    lut_seq_idx_t seqid = LUT_SEQ_IDX_USER_READ;
    uint32_t _id[2] = {0};
    uint8_t  *id = (uint8_t *)&_id;
    int retval = 0;

    FLEXSPI_UpdateLUT(FLEXSPI_BASE, (seqid * LUT_SEQ_SIZE), (const uint32_t *)lut_seq[!!multiline].table, LUT_SEQ_SIZE);

    retval = flexspi_read(0, seqid, _id, 8);
    if (retval == kStatus_Success)
    {
    #if (CONFIG_DEV_MX25UM512_OPI_MODE == 2)
        if (multiline)
        {
            *chipid = (id[0] | (id[2] << 8) | (id[4] << 16));
        }
        else
    #endif
        {
            *chipid = _id[0] & 0x00ffffff;
        }

        DEV_INFO("%s(%p, %d) chipid[0x%08x]", __func__, chipid, multiline, *chipid);
    }

    return retval;
}

static int mx25um512_read_status(uint8_t *status, bool multiline)
{
    const lut_seq_t lut_seq[2] =
    {
        {
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x05, kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 1),
        },
        {
        #if (CONFIG_DEV_MX25UM512_OPI_MODE == 1)
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0x05, kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0xFA),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_8PAD, 32, kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_8PAD, 4),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR, kFLEXSPI_8PAD, 1, 0, 0, 0),
        #else
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x05, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xFA),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 32, kFLEXSPI_Command_DUMMY_DDR, kFLEXSPI_8PAD, 8),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_DDR, kFLEXSPI_8PAD, 1, 0, 0, 0),
        #endif
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

static int mx25um512_write_status(uint8_t status, bool multiline)
{
    const lut_seq_t lut_seq[2] =
    {
        {
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x01, kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 1),
        },
        {
        #if (CONFIG_DEV_MX25UM512_OPI_MODE == 1)
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0x01, kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0xFE),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_8PAD, 32, kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_8PAD, 1),
        #else
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x01, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xFE),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 32, kFLEXSPI_Command_WRITE_DDR, kFLEXSPI_8PAD, 1),
        #endif
        }
    };
    lut_seq_idx_t seqid = LUT_SEQ_IDX_USER_WRITE;
    uint32_t _status = 0;
    int retval = 0;

    FLEXSPI_UpdateLUT(FLEXSPI_BASE, (seqid * LUT_SEQ_SIZE), (const uint32_t *)lut_seq[!!multiline].table, LUT_SEQ_SIZE);

    _status = status;

    retval = flexspi_write(0, seqid, &_status, 1);
    if (retval == kStatus_Success)
    {
        DEV_DEBUG("%s(0x%02x, %d)", __func__, status, multiline);
    }

    return retval;
}

static int mx25um512_wait_status_idle(bool multiline)
{
    uint8_t timeout = 20;
    uint8_t status = 0;
    int retval = 0;

    do {
        mx25um512_read_status(&status, multiline);
        if ((status & 0x01) == 0)
                break;
    } while (--timeout > 0);

    if (timeout < 0)
    {
        DEV_ERROR("wait WIP idle timeout");
        retval = -1;
    }

    return retval;
}

static int mx25um512_write_enable(bool multiline)
{
    const lut_seq_t lut_seq[2] =
    {
        {
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x06, 0, 0, 0),
        },
        {
        #if (CONFIG_DEV_MX25UM512_OPI_MODE == 1)
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0x06, kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0xF9),
        #else
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x06, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xF9),
        #endif
        }
    };
    lut_seq_idx_t seqid = LUT_SEQ_IDX_USER_COMMAND;
    int retval = 0;

    FLEXSPI_UpdateLUT(FLEXSPI_BASE, (seqid * LUT_SEQ_SIZE), (const uint32_t *)lut_seq[!!multiline].table, LUT_SEQ_SIZE);

    retval = flexspi_command(0, seqid);

    return retval;
}

static int mx25um512_read_cr2(uint32_t reg, uint8_t *val, bool multiline)
{
    const lut_seq_t lut_seq[2] =
    {
        {
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x71, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 32),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 1, 0, 0, 0),
        },
        {
        #if (CONFIG_DEV_MX25UM512_OPI_MODE == 1)
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0x71, kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0x8E),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_8PAD, 32, kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_8PAD, 4),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR, kFLEXSPI_8PAD, 1, 0, 0, 0),
        #else
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x71, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x8E),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 32, kFLEXSPI_Command_DUMMY_DDR, kFLEXSPI_8PAD, 8),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_DDR, kFLEXSPI_8PAD, 1, 0, 0, 0),
        #endif
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

static int mx25um512_write_cr2(uint32_t reg, uint8_t val, bool multiline)
{
    const lut_seq_t lut_seq[2] =
    {
        {
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x72, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 32),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 1, 0, 0, 0),
        },
        {
        #if (CONFIG_DEV_MX25UM512_OPI_MODE == 1)
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0x72, kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0x8D),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_8PAD, 32, kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_8PAD, 1),
        #else
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x72, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x8D),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 32, kFLEXSPI_Command_WRITE_DDR, kFLEXSPI_8PAD, 1),
        #endif
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

static void mx25um512_update_lut(void)
{
    const lut_seq_t lut_seq[LUT_MAX_SIZE] =
    {
    #if (CONFIG_DEV_MX25UM512_OPI_MODE == 0)
        [LUT_SEQ_IDX_READ_STATUS] =
        {
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x05, kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 1),
        },
        [LUT_SEQ_IDX_WRITE_ENABLE] =
        {
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x06, 0, 0, 0),
        },
        [LUT_SEQ_IDX_ERASE_SECTOR] =
        {
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x21, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 32),
        },
        [LUT_SEQ_IDX_READ_PAGE] =
        {
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x13, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 32),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 4, 0, 0, 0),
        },
        [LUT_SEQ_IDX_WRITE_PAGE] =
        {
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x12, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 32),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 4, 0, 0, 0),
        },
    #elif (CONFIG_DEV_MX25UM512_OPI_MODE == 1)
        [LUT_SEQ_IDX_READ_STATUS] =
        {
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0x05, kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0xFA),
		    FLEXSPI_LUT_SEQ(kFLEXSPI_Command_RADDR_SDR,	kFLEXSPI_8PAD, 32, kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_8PAD, 4),
		    FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR, kFLEXSPI_8PAD, 1, 0, 0, 0),
        },
        [LUT_SEQ_IDX_WRITE_ENABLE] =
        {
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0x06, kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0xF9),
        },
        [LUT_SEQ_IDX_ERASE_SECTOR] =
        {
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0x21, kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0xDE),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_8PAD, 32, 0, 0, 0),
        },
        [LUT_SEQ_IDX_READ_PAGE] =
        {
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0xEC, kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0x13),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_8PAD, 32, kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_8PAD, 20),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR, kFLEXSPI_8PAD, 4, 0, 0, 0),
        },
        [LUT_SEQ_IDX_WRITE_PAGE] =
        {
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0x12, kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0xED),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_8PAD, 32, kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_8PAD, 4),
        },
    #elif (CONFIG_DEV_MX25UM512_OPI_MODE == 2)
        [LUT_SEQ_IDX_READ_STATUS] =
        {
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x05, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xFA),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 32, kFLEXSPI_Command_DUMMY_DDR, kFLEXSPI_8PAD, 8),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_DDR, kFLEXSPI_8PAD, 1, 0, 0, 0),
        },
        [LUT_SEQ_IDX_WRITE_ENABLE] =
        {
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x06, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xF9),
        },
        [LUT_SEQ_IDX_ERASE_SECTOR] =
        {
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x21, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xDE),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 32, 0, 0, 0),
        },
        [LUT_SEQ_IDX_READ_PAGE] =
        {
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xEE, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x11),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 32, kFLEXSPI_Command_DUMMY_DDR, kFLEXSPI_8PAD, 40),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_DDR, kFLEXSPI_8PAD, 4, 0, 0, 0),
        },
        [LUT_SEQ_IDX_WRITE_PAGE] =
        {
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x12, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xED),
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 32, kFLEXSPI_Command_WRITE_DDR, kFLEXSPI_8PAD, 4),
        },
    #else
        #error "Unknown CONFIG_DEV_MX25UM512_OPI_MODE"
    #endif
    };

    for (int i = 0; i < LUT_MAX_SIZE; i++)
    {
        DEV_DEBUG("LUT[%02d]: 0x%08x 0x%08x 0x%08x 0x%08x", i,
                    lut_seq[i].table[0], lut_seq[i].table[1], lut_seq[i].table[2], lut_seq[i].table[3]);
    }

    FLEXSPI_UpdateLUT(FLEXSPI_BASE, 0, (const uint32_t *)lut_seq, (LUT_MAX_SIZE * LUT_SEQ_SIZE));
}

int mx25um512_probe(void)
{
    uint32_t chipid = 0; // 0x003A81C2
    int retval = 0;

    DEV_ENTER("");

#if (CONFIG_DEV_MX25UM512_OPI_MODE == 0)
    retval = mx25um512_read_chipid(&chipid, false);
    if ((retval == 0) && (chipid == 0x003A81C2))
    {
        uint8_t value = 0;
        mx25um512_read_cr2(0x0000, &value, false);
        mx25um512_read_cr2(0x0200, &value, true);

        mx25um512_update_lut();
    }
    else
    {
        retval = -1;
    }
#else
    retval = mx25um512_read_chipid(&chipid, false);
    if ((retval == 0) && (chipid == 0x003A81C2))
    { // Switch to DDR OPI
        mx25um512_write_enable(false);
        mx25um512_write_status(0, false);
        retval = mx25um512_wait_status_idle(false);
        if (retval != 0) {
            goto ERR_EXIT;
        }

        mx25um512_write_enable(false);
        mx25um512_write_cr2(0x0000, CONFIG_DEV_MX25UM512_OPI_MODE, false);
        retval = mx25um512_wait_status_idle(true);
        if (retval != 0) {
            goto ERR_EXIT;
        }
    }

    retval = mx25um512_read_chipid(&chipid, true);
    if ((retval == 0) && (chipid == 0x003A81C2))
    {
        uint8_t value = 0;
        mx25um512_read_cr2(0x0000, &value, true);

        mx25um512_update_lut();
    }
    else
    {
        retval = -1;
    }

ERR_EXIT:
#endif

    DEV_LEAVE("%d", retval);
    return retval;
}

#endif
