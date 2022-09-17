
#include "fsl_power.h"
#include "norflash_drv.h"

#if defined(CONFIG_DRV_FLEXSPI)

#include "log.h"
#include "norflash_lut.h"

static const flexspi_device_config_t devcfg =
{
    .flexspiRootClk       = 99000000,
    .flashSize            = (FLASH_CAPACITY / 1024),
    .CSIntervalUnit       = kFLEXSPI_CsIntervalUnit1SckCycle,
    .CSInterval           = 2,
    .CSHoldTime           = 3,
    .CSSetupTime          = 3,
    .dataValidTime        = 2,
    .columnspace          = 0,
    .enableWordAddress    = 0,
    .AWRSeqIndex          = LUT_SEQ_IDX_WRITE_PAGE,
    .AWRSeqNumber         = 1,
    .ARDSeqIndex          = LUT_SEQ_IDX_READ_PAGE,
    .ARDSeqNumber         = 1,
    .AHBWriteWaitUnit     = kFLEXSPI_AhbWriteWaitUnit2AhbCycle,
    .AHBWriteWaitInterval = 0,
};

int flexspi_init(void)
{
    flexspi_config_t config;
    int retval = 0;

    DRV_ENTER("");

    CLOCK_AttachClk(kAUX0_PLL_to_FLEXSPI0_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivFlexspi0Clk, 3);

    /*Get FLEXSPI default settings and configure the flexspi. */
    FLEXSPI_GetDefaultConfig(&config);

    /*Set AHB buffer size for reading data through AHB bus. */
    config.ahbConfig.enableAHBPrefetch    = true;
    config.ahbConfig.enableAHBBufferable  = true;
    config.ahbConfig.enableReadAddressOpt = true;
    config.ahbConfig.enableAHBCachable    = true;
    config.rxSampleClock                  = kFLEXSPI_ReadSampleClkLoopbackFromDqsPad;
    FLEXSPI_Init(FLEXSPI_BASE, &config);

    /* Configure flash settings according to serial flash feature. */
    FLEXSPI_SetFlashConfig(FLEXSPI_BASE, (flexspi_device_config_t *)&devcfg, FLEXSPI_PORT);

    DRV_LEAVE("%d", retval);
    return retval;
}

int flexspi_command(uint32_t addr, lut_seq_idx_t seqid)
{
    flexspi_transfer_t flashXfer;
    status_t retval = 0;

    DRV_ENTER("%u, %d", addr, seqid);

    flashXfer.deviceAddress = addr;
    flashXfer.port          = FLEXSPI_PORT;
    flashXfer.cmdType       = kFLEXSPI_Command;
    flashXfer.seqIndex      = seqid;
    flashXfer.SeqNumber     = 1;

    retval = FLEXSPI_TransferBlocking(FLEXSPI_BASE, &flashXfer);
    if (retval != kStatus_Success)
    {
        DRV_ERROR("%s(0x%08x, %d) fail[%d]", __func__, addr, seqid, retval);
        FLEXSPI_SoftwareReset(FLEXSPI_BASE);
    }

    DRV_LEAVE("%d", retval);
    return retval;
}

int flexspi_read(uint32_t addr, lut_seq_idx_t seqid, uint32_t *data, size_t size)
{
    flexspi_transfer_t flashXfer;
    status_t retval = 0;

    DRV_ENTER("%u, %d, %p, %d", addr, seqid, data, size);

    flashXfer.deviceAddress = addr;
    flashXfer.port          = FLEXSPI_PORT;
    flashXfer.cmdType       = kFLEXSPI_Read;
    flashXfer.seqIndex      = seqid;
    flashXfer.SeqNumber     = 1;
    flashXfer.data          = data;
    flashXfer.dataSize      = size;

    retval = FLEXSPI_TransferBlocking(FLEXSPI_BASE, &flashXfer);
    if (retval != kStatus_Success)
    {
        DRV_ERROR("%s(0x%08x, %d, %p, %d) fail[%d]", __func__, addr, seqid, data, size, retval);
        FLEXSPI_SoftwareReset(FLEXSPI_BASE);
    }

    DRV_LEAVE("%d", retval);
    return retval;
}

int flexspi_write(uint32_t addr, lut_seq_idx_t seqid, uint32_t *data, size_t size)
{
    flexspi_transfer_t flashXfer;
    status_t retval = 0;

    DRV_ENTER("%u, %d, %p, %d", addr, seqid, data, size);

    flashXfer.deviceAddress = addr;
    flashXfer.port          = FLEXSPI_PORT;
    flashXfer.cmdType       = kFLEXSPI_Write;
    flashXfer.seqIndex      = seqid;
    flashXfer.SeqNumber     = 1;
    flashXfer.data          = data;
    flashXfer.dataSize      = size;

    retval = FLEXSPI_TransferBlocking(FLEXSPI_BASE, &flashXfer);
    if (retval != kStatus_Success)
    {
        DRV_ERROR("%s(0x%08x, %d, %p, %d) fail[%d]", __func__, addr, seqid, data, size, retval);
        FLEXSPI_SoftwareReset(FLEXSPI_BASE);
    }

    DRV_LEAVE("%d", retval);
    return retval;
}

int flexspi_read_status(uint8_t *status)
{
    uint32_t _status = 0;
    int retval = 0;

    retval = flexspi_read(0, LUT_SEQ_IDX_READ_STATUS, &_status, 1);
    if (retval == kStatus_Success)
    {
        *status = (uint8_t)(_status & 0xff);
    }

    return retval;
}

int flexspi_write_enable(void)
{
    return flexspi_command(0, LUT_SEQ_IDX_WRITE_ENABLE);
}

int flexspi_erase_sector(uint32_t addr)
{
    return flexspi_command(addr, LUT_SEQ_IDX_ERASE_SECTOR);
}

int flexspi_read_page(uint32_t addr, uint8_t *data, size_t size)
{
    return flexspi_read(addr, LUT_SEQ_IDX_READ_PAGE, (uint32_t *)data, size);
}

int flexspi_write_page(uint32_t addr, uint8_t *data, size_t size)
{
    if (size > FLASH_BLOCK_SIZE)
    {
        DRV_ERROR("%s(%u, %p, %d), Invalid argument.", __func__, addr, data, size);
        return -1;
    }

    return flexspi_write(addr, LUT_SEQ_IDX_WRITE_PAGE, (uint32_t *)data, size);
}

#endif
