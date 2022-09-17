/*
 * Copyright (c) 2007-2015 Freescale Semiconductor, Inc.
 * Copyright 2018-2019 NXP
 *
 * License: NXP LA_OPT_NXP_Software_License
 *
 * NXP Confidential. This software is owned or controlled by NXP and may
 * only be used strictly in accordance with the applicable license terms.
 * By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that
 * you have read, and that you agree to comply with and are bound by,
 * such license terms.  If you do not agree to be bound by the applicable
 * license terms, then you may not retain, install, activate or otherwise
 * use the software.  This code may only be used in a microprocessor,
 * microcontroller, sensor or digital signal processor ("NXP Product")
 * supplied directly or indirectly from NXP.  See the full NXP Software
 * License Agreement in license/LA_OPT_NXP_Software_License.pdf
 *
 * FreeMASTER Communication Driver - S12Z msCAN low-level driver
 */

#ifndef __FREEMASTER_S12Z_MSCAN_H
#define __FREEMASTER_S12Z_MSCAN_H

/******************************************************************************
* Required header files include check
******************************************************************************/
#ifndef __FREEMASTER_H
#error Please include the freemaster.h master header file before the freemaster_s12z_mscan.h
#endif

/******************************************************************************
* Adapter configuration
******************************************************************************/

#ifdef __cplusplus
  extern "C" {
#endif
      
/******************************************************************************
* Types definition
******************************************************************************/

/******************************************************************************
* inline functions
******************************************************************************/

/******************************************************************************
* Global API functions
******************************************************************************/
      
void FMSTR_CanSetBaseAddress(FMSTR_ADDR base);
void FMSTR_CanIsr(void);

#ifdef __cplusplus
  }
#endif

#endif /* __FREEMASTER_S12Z_MSCAN_H */

