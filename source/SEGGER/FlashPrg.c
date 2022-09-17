/*********************************************************************
*            (c) 1995 - 2018 SEGGER Microcontroller GmbH             *
*                        The Embedded Experts                        *
*                           www.segger.com                           *
**********************************************************************
----------------------------------------------------------------------
File    : FlashPrg.c
Purpose : Implementation of RAMCode template
--------  END-OF-HEADER  ---------------------------------------------
*/
#include "FlashOS.h"

#include "pin_mux.h"
#include "clock_config.h"

#include "log.h"
#include "watchdog.h"
#include "norflash.h"

/*********************************************************************
*
*       Defines (configurable)
*
**********************************************************************
*/

//
// Some flash types require a native verify function as the memory is not memory mapped available (e.g. eMMC flashes).
// If the verify function is implemented in the algorithm, it will be used by the J-Link DLL during compare / verify
// independent of what verify type is configured in the J-Link DLL.
// Please note, that SEGGER does not recommend to use this function if the flash can be memory mapped read
// as this may can slow-down the compare / verify step.
//
#define SUPPORT_NATIVE_VERIFY        (0)
#define SUPPORT_NATIVE_READ_FUNCTION (1)
#define SUPPORT_BLANK_CHECK          (0)
#define SUPPORT_ERASE_CHIP           (0)
#define SUPPORT_SEGGER_OPEN_Program  (1)
#define SUPPORT_SEGGER_OPEN_ERASE    (1)

/*********************************************************************
*
*       Types
*
**********************************************************************
*/

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
//
// We use this dummy variable to make sure that the PrgData
// section is present in the output elf-file as this section
// is mandatory in current versions of the J-Link DLL
//
static volatile int _Dummy;

static U8 *DataBuff = 0x300000;

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/

/*********************************************************************
*
*       Init
*
*  Function description
*    Handles the initialization of the flash module.
*
*  Parameters
*    Addr: Flash base address
*    Freq: Clock frequency in Hz
*    Func: Specifies the action followed by Init() (e.g.: 1 - Erase, 2 - Program, 3 - Verify / Read)
*
*  Return value
*    0 O.K.
*    1 Error
*/
int Init(U32 Addr, U32 Freq, U32 Func) {
    (void)Addr;
    (void)Freq;
    (void)Func;
    //
    // Init code
    //
    BOARD_InitPins();
    BOARD_BootClockRUN();

    log_init();

    int retval = 0;
    PRG_ENTER("0x%08x, %u, %u", Addr, Freq, Func);

    watchdog_initialize();
    watchdog_start();

    retval = norflash_init();
    if (retval != 0) {
        retval  = 1;
    }

    PRG_LEAVE("%d", retval);
    return retval;
}

/*********************************************************************
*
*       UnInit
*
*  Function description
*    Handles the de-initialization of the flash module.
*
*  Parameters
*    Func: Caller type (e.g.: 1 - Erase, 2 - Program, 3 - Verify)
*
*  Return value
*    0 O.K.
*    1 Error
*/
int UnInit(U32 Func) {
    (void)Func;
    //
    // Uninit code
    //
    int retval = 0;
    PRG_ENTER("%u", Func);

    watchdog_stop();

    PRG_LEAVE("%d", retval);
    return retval;
}

/*********************************************************************
*
*       EraseSector
*
*  Function description
*    Erases one flash sector.
*
*  Parameters
*    SectorAddr: Absolute address of the sector to be erased
*
*  Return value
*    0 O.K.
*    1 Error
*/
int EraseSector(U32 SectorAddr) {
    //
    // Erase sector code
    //
    int retval = 0;
    PRG_ENTER("0x%08x", SectorAddr);

    watchdog_feed();

    retval = norflash_erase_sector(SectorAddr);
    if (retval != 0) {
        retval  = 1;
    }

#if defined(CONFIG_READBACK_CHECK)
    // U8 DataBuff[FLASH_BLOCK_SIZE];
    U32 NumPages = FLASH_ERASE_SIZE / FLASH_BLOCK_SIZE;

    for (int i = 0; i < NumPages; i++) {
        retval = norflash_read_page(SectorAddr, DataBuff, FLASH_BLOCK_SIZE);
        if (retval != 0) {
            retval  = 1;
            break;
        }

        for (int j = 0; j < FLASH_BLOCK_SIZE; j++)
        {
            if (DataBuff[j] != 0xFF) {
                PRG_ERROR("Addr[0x%08x] Data(%d)[0x%02x != 0xFF]", SectorAddr, j, DataBuff[j]);
                retval = 1;
                break;
            }
        }

        SectorAddr += FLASH_BLOCK_SIZE;
    }
#endif

    PRG_LEAVE("%d", retval);
    return retval;
}

/*********************************************************************
*
*       ProgramPage
*
*  Function description
*    Programs one flash page.
*
*  Parameters
*    DestAddr: Destination address
*    NumBytes: Number of bytes to be programmed (always a multiple of program page size, defined in FlashDev.c)
*    pSrcBuff: Point to the source buffer
*
*  Return value
*    0 O.K.
*    1 Error
*/
int ProgramPage(U32 DestAddr, U32 NumBytes, U8 *pSrcBuff) {
    U32 NumPages = 0;
    int retval = 0;
    PRG_ENTER("0x%08x, %u, %p", DestAddr, NumBytes, pSrcBuff);

    watchdog_feed();

    NumPages = NumBytes / FLASH_BLOCK_SIZE;

    for (U32 i = 0; i < NumPages; i++) {
        retval = norflash_write_page(DestAddr, pSrcBuff, FLASH_BLOCK_SIZE);
        if (retval != 0) {
            retval  = 1;
            break;
        }

    #if defined(CONFIG_READBACK_CHECK)
        // U8 DataBuff[FLASH_BLOCK_SIZE];
        retval = norflash_read_page(DestAddr, DataBuff, FLASH_BLOCK_SIZE);
        if (retval != 0) {
            retval  = 1;
            break;
        }

        for (int j = 0; j < FLASH_BLOCK_SIZE; j++)
        {
            if (DataBuff[j] != pSrcBuff[j]) {
                PRG_ERROR("Addr[0x%08x] Data(%d)[0x%02x != 0x%02x]", DestAddr, j, DataBuff[j], pSrcBuff[j]);
                retval = 1;
                break;
            }
        }
    #endif

        DestAddr += FLASH_BLOCK_SIZE;
        pSrcBuff += FLASH_BLOCK_SIZE;
    }

    PRG_LEAVE("%d", retval);
    return retval;
}

/*********************************************************************
*
*       Verify
*
*  Function description
*    Compares a specified number of bytes of a provided data
*    buffer with the content of the device
*
*  Parameters
*    Addr: Start address in memory which should be compared
*    NumBytes: Number of bytes to be compared
*    pBuff: Pointer to the data to be compared
*
*  Return value
*    == (Addr + NumBytes): O.K.
*    != (Addr + NumBytes): *not* O.K. (ideally the fail address is returned)
*
*/
#if SUPPORT_NATIVE_VERIFY
U32 Verify(U32 Addr, U32 NumBytes, U8 *pBuff) {
    U32 ReadBytes = 0;
    // U8  DataBuff[FLASH_BLOCK_SIZE];
    U32 retval = 0;
    PRG_ENTER("0x%08x, %u, %p", Addr, NumBytes, pBuff);

    watchdog_feed();

    for (ReadBytes = 0; ReadBytes < NumBytes; ReadBytes += FLASH_BLOCK_SIZE) {
        PRG_DEBUG("norflash_read_page(0x%08x, %p, %d)", (Addr + ReadBytes), DataBuff, FLASH_BLOCK_SIZE);
        retval = norflash_read_page((Addr + ReadBytes), DataBuff, FLASH_BLOCK_SIZE);
        if (retval != 0) {
            retval  = Addr + ReadBytes;
            break;
        }

        for (int i = 0; i < MIN((NumBytes - ReadBytes), FLASH_BLOCK_SIZE); i++) {
            if (DataBuff[i] != pBuff[i]) {
                retval = Addr + ReadBytes + i;
                break;
            }
        }

        pBuff += FLASH_BLOCK_SIZE;
    }

    if (retval == 0) {
        retval  = Addr + NumBytes;
    }

    PRG_LEAVE("0x%08x", retval);
    return retval;
}
#endif

/*********************************************************************
*
*       BlankCheck
*
*  Function description
*    Checks if a memory region is blank
*
*  Parameters
*    Addr: Blank check start address
*    NumBytes: Number of bytes to be checked
*    BlankData: Pointer to the destination data
*
*  Return value
*    0: O.K., blank
*    1: O.K., *not* blank
*    < 0: Error
*
*/
#if SUPPORT_BLANK_CHECK
int BlankCheck(U32 Addr, U32 NumBytes, U8 BlankData) {
    // U8  DataBuff[FLASH_BLOCK_SIZE];
    U32 ReadBytes = 0;
    int retval = 0;
    PRG_ENTER("0x%08x, %u, 0x%02x", Addr, NumBytes, BlankData);

    watchdog_feed();

    for (ReadBytes = 0; ReadBytes < NumBytes; ReadBytes += FLASH_BLOCK_SIZE) {
        PRG_DEBUG("norflash_read_page(0x%08x, %p, %d)", (Addr + ReadBytes), DataBuff, FLASH_BLOCK_SIZE);
        retval = norflash_read_page((Addr + ReadBytes), DataBuff, FLASH_BLOCK_SIZE);
        if (retval != 0) {
            retval  = -1;
            break;
        }

        for (int i = 0; i < MIN((NumBytes - ReadBytes), FLASH_BLOCK_SIZE); i++) {
            if (DataBuff[i] != BlankData) {
                retval = 1;
                break;
            }
        }
    }

    PRG_LEAVE("%d", retval);
    return retval;
}
#endif

/*********************************************************************
*
*       EraseChip
*
*  Function description
*    Erases the entire flash
*
*  Return value
*    0: O.K.
*    1: Error
*/
#if SUPPORT_ERASE_CHIP
int EraseChip(void) {
    //
    // Erase chip code
    //
    return 1;
}
#endif

/*********************************************************************
*
*       SEGGER_OPEN_Read
*
*  Function description
*    Reads a specified number of bytes into the provided buffer
*
*  Parameters
*    Addr: Start read address
*    NumBytes: Number of bytes to be read
*    pBuff: Pointer to the destination data
*
*  Return value
*    >= 0: O.K., NumBytes read
*    <  0: Error
*
*/
#if SUPPORT_NATIVE_READ_FUNCTION
int SEGGER_OPEN_Read(U32 Addr, U32 NumBytes, U8 *pDestBuff) {
    //
    // Read function
    // Add your code here...
    //
    int retval = 0;
    PRG_ENTER("0x%08x, %u, %p", Addr, NumBytes, pDestBuff);

    watchdog_feed();

    for (U32 ReadBytes = 0; ReadBytes < NumBytes; ReadBytes += FLASH_BLOCK_SIZE) {
        retval = norflash_read_page(Addr, pDestBuff, FLASH_BLOCK_SIZE);
        if (retval != 0) {
            retval  = -1;
            break;
        }

        // PRG_INFO("read_page(0x%08x, %p, %d) data(0x%02x 0x%02x 0x%02x 0x%02x)",
        //         Addr, pDestBuff, FLASH_BLOCK_SIZE, pDestBuff[0], pDestBuff[1], pDestBuff[2], pDestBuff[3]);
        Addr += FLASH_BLOCK_SIZE;
        pDestBuff += FLASH_BLOCK_SIZE;
    }

    if (retval == 0) {
        retval = NumBytes;
    }

    PRG_LEAVE("%d", retval);
    return retval;
}
#endif

/*********************************************************************
*
*       SEGGER_OPEN_Program
*
*  Function description
*    Programs a specified number of bytes into the target flash.
*    NumBytes is either FlashDevice.PageSize or a multiple of it.
*
*  Notes
*    (1) This function can rely on that at least FlashDevice.PageSize will be passed
*    (2) This function must be able to handle multiple of FlashDevice.PageSize
*
*  Parameters
*    Addr: Start read address
*    NumBytes: Number of bytes to be read
*    pBuff: Pointer to the destination data
*
*  Return value
*    0 O.K.
*    1 Error
*
*/
#if SUPPORT_SEGGER_OPEN_Program
int SEGGER_OPEN_Program(U32 DestAddr, U32 NumBytes, U8 *pSrcBuff) {
    U32 NumPages = 0;
    int retval = 0;
    PRG_ENTER("0x%08x, %u, %p", DestAddr, NumBytes, pSrcBuff);

    NumPages = NumBytes / FLASH_BLOCK_SIZE;

    for (U32 i = 0; i < NumPages; i++) {
        retval = ProgramPage(DestAddr, FLASH_BLOCK_SIZE, pSrcBuff);
        if (retval != 0) {
            return 1;
        }

        DestAddr += FLASH_BLOCK_SIZE;
        pSrcBuff += FLASH_BLOCK_SIZE;
    }

    PRG_LEAVE("%d", retval);
    return retval;
}
#endif

/*********************************************************************
*
*       SEGGER_OPEN_Erase
*
*  Function description
*    Erases one or more flash sectors
*
*  Notes
*    (1) This function can rely on that at least one sector will be passed
*    (2) This function must be able to handle multiple sectors at once
*    (3) This function can rely on that only multiple sectors of the same sector
*        size will be passed. (e.g. if the device has two sectors with different
*        sizes, the DLL will call this function two times with NumSectors = 1)
*
*  Parameters
*    SectorAddr: Address of the start sector to be erased
*    SectorIndex: Index of the start sector to be erased
*    NumSectors: Number of sectors to be erased. At least 1 sector is passed.
*
*  Return value
*    0 O.K.
*    1 Error
*
*/
#if SUPPORT_SEGGER_OPEN_ERASE
int SEGGER_OPEN_Erase(U32 SectorAddr, U32 SectorIndex, U32 NumSectors) {
    int retval = 0;
    PRG_ENTER("0x%08x, %u, %u", SectorAddr, SectorIndex, NumSectors);

    for (int i = 0; i < NumSectors; i++) {
        retval = EraseSector(SectorAddr);
        if (retval != 0) {
            retval = 1;
            break;
        }

        SectorAddr += FLASH_ERASE_SIZE;
    }

    PRG_LEAVE("%d", retval);
    return retval;
}
#endif

/**************************** End of file ***************************/
