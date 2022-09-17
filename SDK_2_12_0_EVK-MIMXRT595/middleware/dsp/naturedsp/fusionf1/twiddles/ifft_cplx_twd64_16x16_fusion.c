/* ------------------------------------------------------------------------ */
/* Copyright (c) 2016 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs (�Cadence    */
/* Libraries�) are the copyrighted works of Cadence Design Systems Inc.	    */
/* Cadence IP is licensed for use with Cadence processor cores only and     */
/* must not be used for any other processors and platforms. Your use of the */
/* Cadence Libraries is subject to the terms of the license agreement you   */
/* have entered into with Cadence Design Systems, or a sublicense granted   */
/* to you by a direct Cadence licensee.                                     */
/* ------------------------------------------------------------------------ */
/*  IntegrIT, Ltd.   www.integrIT.com, info@integrIT.com                    */
/*                                                                          */
/* DSP Library                                                              */
/*                                                                          */
/* This library contains copyrighted materials, trade secrets and other     */
/* proprietary information of IntegrIT, Ltd. This software is licensed for  */
/* use with Cadence processor cores only and must not be used for any other */
/* processors and platforms. The license to use these sources was given to  */
/* Cadence, Inc. under Terms and Condition of a Software License Agreement  */
/* between Cadence, Inc. and IntegrIT, Ltd.                                 */
/* ------------------------------------------------------------------------ */
/*          Copyright (C) 2015-2016 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
/*
	NatureDSP_Signal library. FFT part
    C code with generic optimization

    twiddle factors for 16x16 complex fwd/rev FFT transforms
*/

#include "NatureDSP_Signal.h"
#include "fft_cplx_twiddles.h"
#include "common.h"

 // This tables generated by next MATLAB script:
/*
function print_cfft16x16_twd(N, isInv)
% print_cfft_twd(N, format)
% N - fft size, must be power of 2

  format = 'int16_t'; 
  if isInv==0
    fname = sprintf('fft_cplx_twd%d_16x16_fusion.c', N);
  else
    fname = sprintf('ifft_cplx_twd%d_16x16_fusion.c', N);
  end
  f = fopen(fname,'wt');
  %% Print current script to output file
  script_name = strcat(mfilename, '.m'); 
  fs = fopen(script_name, 'r'); 
  txt = fread(fs, inf, 'uint8=>char').';
  txt = regexprep(txt,'\r\n|\n|\r','\n');
  txt = regexprep(txt,' *| ',' ');
  fclose(fs); 
  fprintf(f, '\n   This tables generated by next MATLAB script:\n *\n');
  fprintf(f, '%s', txt); 
  fprintf(f, '\n \n');
  %% Display output file name
  if f>0
      fprintf('write %s\n', fname);
  end
  if isInv==0
    twd = exp(-2j*pi*[1;2;3]*(0:N/4-1)/N);
  else
    twd = exp(2j*pi*[1;2;3]*(0:N/4-1)/N);
  end
  
  %% Calculate twiddles
  tmp3 = reshape(twd(:), 6, numel(twd(:))/6);
  tmp4 = tmp3; 
  tmp4(1:2:end,:) = tmp3(1:end/ 2,:);
  tmp4(2:2:end,:) = tmp3(end/ 2+1:end, :); 
  
  twd = tmp4(:);
  twd_ri = reshape([imag(twd(:).');real(twd(:).')],1,2*numel(twd));
 %% Print values to output file  
   if isInv==0
        fprintf(f, '\nALIGN(8) static const %s _fft_twiddle_table_%d_[%d] = \n{\n', format, N, length(twd_ri));
   else
        fprintf(f, '\nALIGN(8) static const %s _ifft_twiddle_table_%d_[%d] = \n{\n', format, N, length(twd_ri));
   end
  tmp = round(twd_ri*2^15);

  for i=1:4: length(tmp)
    fprintf(f, '\n    ');
    fprintf(f, '(int16_t)0x%04X, ', typecast(int16(tmp(i:i+4-1)), 'uint16')); 
  end
  
  fprintf(f, '\n};\n');
  fclose(f);

*/
#if (XCHAL_HAVE_FUSION_16BIT_BASEBAND)
ALIGN(8)  const int16_t _ifft_twiddle_table_64_[96] = 
{

    (int16_t)0x0000, (int16_t)0x7FFF, (int16_t)0x0C8C, (int16_t)0x7F62, 
    (int16_t)0x0000, (int16_t)0x7FFF, (int16_t)0x18F9, (int16_t)0x7D8A, 
    (int16_t)0x0000, (int16_t)0x7FFF, (int16_t)0x2528, (int16_t)0x7A7D, 
    (int16_t)0x18F9, (int16_t)0x7D8A, (int16_t)0x2528, (int16_t)0x7A7D, 
    (int16_t)0x30FC, (int16_t)0x7642, (int16_t)0x471D, (int16_t)0x6A6E, 
    (int16_t)0x471D, (int16_t)0x6A6E, (int16_t)0x62F2, (int16_t)0x5134, 
    (int16_t)0x30FC, (int16_t)0x7642, (int16_t)0x3C57, (int16_t)0x70E3, 
    (int16_t)0x5A82, (int16_t)0x5A82, (int16_t)0x6A6E, (int16_t)0x471D, 
    (int16_t)0x7642, (int16_t)0x30FC, (int16_t)0x7F62, (int16_t)0x0C8C, 
    (int16_t)0x471D, (int16_t)0x6A6E, (int16_t)0x5134, (int16_t)0x62F2, 
    (int16_t)0x7642, (int16_t)0x30FC, (int16_t)0x7D8A, (int16_t)0x18F9, 
    (int16_t)0x7D8A, (int16_t)0xE707, (int16_t)0x70E3, (int16_t)0xC3A9, 
    (int16_t)0x5A82, (int16_t)0x5A82, (int16_t)0x62F2, (int16_t)0x5134, 
    (int16_t)0x7FFF, (int16_t)0x0000, (int16_t)0x7D8A, (int16_t)0xE707, 
    (int16_t)0x5A82, (int16_t)0xA57E, (int16_t)0x3C57, (int16_t)0x8F1D, 
    (int16_t)0x6A6E, (int16_t)0x471D, (int16_t)0x70E3, (int16_t)0x3C57, 
    (int16_t)0x7642, (int16_t)0xCF04, (int16_t)0x6A6E, (int16_t)0xB8E3, 
    (int16_t)0x18F9, (int16_t)0x8276, (int16_t)0xF374, (int16_t)0x809E, 
    (int16_t)0x7642, (int16_t)0x30FC, (int16_t)0x7A7D, (int16_t)0x2528, 
    (int16_t)0x5A82, (int16_t)0xA57E, (int16_t)0x471D, (int16_t)0x9592, 
    (int16_t)0xCF04, (int16_t)0x89BE, (int16_t)0xAECC, (int16_t)0x9D0E, 
    (int16_t)0x7D8A, (int16_t)0x18F9, (int16_t)0x7F62, (int16_t)0x0C8C, 
    (int16_t)0x30FC, (int16_t)0x89BE, (int16_t)0x18F9, (int16_t)0x8276, 
    (int16_t)0x9592, (int16_t)0xB8E3, (int16_t)0x8583, (int16_t)0xDAD8, 
};

static const cint32_ptr seqfft_[] =
{
    (cint32_ptr)_ifft_twiddle_table_64_,
    (cint32_ptr)_ifft_twiddle_table_16_
};


static const tFftDescr descr = { 64, 0, (cint32_ptr)seqfft_ };

const fft_handle_t cifft16x16_64=(const void*)&descr;
const fft_handle_t rifft16x16_128=(const void*)&descr;
#endif
