/*********************************************************************
*            (c) 1995 - 2018 SEGGER Microcontroller GmbH             *
*                        The Embedded Experts                        *
*                           www.segger.com                           *
**********************************************************************
----------------------------------------------------------------------
File    : FlashDev.c
Purpose : Flash device description Template
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "FlashOS.h"
#include "norflash.h"

struct FlashDevice const FlashDevice __attribute__ ((section ("DevDscr"))) =  {
  ALGO_VERSION,              // Algo version
  "MIMXRT595-NOR",           // Flash device name
  EXTSPI,                    // Flash device type
  0x08000000,                // Flash base address
  FLASH_CAPACITY,            // Total flash device size in Bytes (64M)
  FLASH_BLOCK_SIZE,          // Page Size (number of bytes that will be passed to ProgramPage(). May be multiple of min alignment in order to reduce overhead for calling ProgramPage multiple times
  0,                         // Reserved, should be 0
  0xFF,                      // Flash erased value
  100,                       // Program page timeout in ms
  6000,                      // Erase sector timeout in ms
  //
  // Flash sector layout definition
  //
  FLASH_ERASE_SIZE, 0x00000000,   // 4 KB
	FLASH_ERASE_SIZE, 0x02000000,
  0xFFFFFFFF, 0xFFFFFFFF    // Indicates the end of the flash sector layout. Must be present.
};
