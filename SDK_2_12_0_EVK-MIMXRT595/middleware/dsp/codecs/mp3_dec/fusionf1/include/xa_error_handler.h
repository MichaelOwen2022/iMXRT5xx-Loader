/*
 * Copyright (c) 2006-2019 by Cadence Design Systems, Inc.  ALL RIGHTS RESERVED.
 * These coded instructions, statements, and computer programs are the
 * copyrighted works and confidential proprietary information of
 * Cadence Design Systems Inc.  They may be adapted and modified by bona fide
 * purchasers for internal use, but neither the original nor any adapted
 * or modified version may be disclosed or distributed to third parties
 * in any manner, medium, or form, in whole or in part, without the prior
 * written consent of Cadence Design Systems Inc.  This software and its
 * derivatives are to be executed solely on products incorporating a Cadence
 * Design Systems processor.
 */


#ifndef __XA_ERROR_HANDLER_H__
#define __XA_ERROR_HANDLER_H__

/*****************************************************************************/
/* File includes                                                             */
/*  xa_type_def.h                                                            */
/*  xa_error_standards.h                                                     */
/*****************************************************************************/

/* these definitions are used by error handling function         */
/* the error handler will work on a structure which identifies a */
/* particular error with a module, context and error_code        */

/*****************************************************************************/
/* Constant hash defines                                                     */
/*****************************************************************************/
#define XA_ERROR_NON_FATAL_IDX		0x0
#define XA_ERROR_FATAL_IDX		0x1

#define XA_ERROR_CLASS_0		0x0
#define XA_ERROR_CLASS_1		0x1
#define XA_ERROR_CLASS_2		0x2
#define XA_ERROR_CLASS_3		0x3
#define XA_ERROR_CLASS_4		0x4
#define XA_ERROR_CLASS_5		0x5
#define XA_ERROR_CLASS_6		0x6
#define XA_ERROR_CLASS_7		0x7
#define XA_ERROR_CLASS_8		0x8
#define XA_ERROR_CLASS_9		0x9
#define XA_ERROR_CLASS_A		0xA
#define XA_ERROR_CLASS_B		0xB
#define XA_ERROR_CLASS_C		0xC
#define XA_ERROR_CLASS_D		0xD
#define XA_ERROR_CLASS_E		0xE
#define XA_ERROR_CLASS_F		0xF

/* each module, hence, needs to copy the following structure          */
/* the first index is for FATAL/NONFATAL                              */
/* the second index is for the classes                                */
/* then in a module specific initialization, fill in the following    */
/* structure with the pointers to the particular error message arrays */

/*****************************************************************************/
/* Type definitions                                                          */
/*****************************************************************************/
typedef struct {
  const char  *pb_module_name;
  const char  *ppb_class_names[16];
  const char **ppppb_error_msg_pointers[2][16];
} xa_error_info_struct;

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
/* this error handler maps the code generated by a module to a error string  */
/* pb_context is a string to specify where the module broke                  */
/* Note that this function declaration logically belongs to the calling      */
/* program; it is not used in the codec library.                             */
XA_ERRORCODE xa_error_handler(xa_error_info_struct *p_mod_err_info, 
			      const char *pb_context,
			      XA_ERRORCODE code);

/*****************************************************************************/
/* Macro functions                                                           */
/*****************************************************************************/
/* the following macro does a one-line job of returning back to the parent   */
/* in case a fatal error occurs after calling the error handler function     */
/* Note that this macro logically belongs to the calling program; it         */
/* is not used in the codec library.                                         */
#define _XA_HANDLE_ERROR(p_mod_err_info, context, e) \
	if((e) != XA_NO_ERROR) \
	{ \
		xa_error_handler((p_mod_err_info), (context), (e)); \
		if((e) & XA_FATAL_ERROR) \
			return (e); \
	}
		
#endif /* __XA_ERROR_HANDLER_H__ */
