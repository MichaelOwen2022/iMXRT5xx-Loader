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
  NatureDSP Signal Processing Library. IIR part
    Bi-quad Real Block IIR, 32x16-bit, Direct Form II
    Optimized code for Fusion
*/

/*-------------------------------------------------------------------------
  Bi-quad Block IIR
  Computes a IIR filter (cascaded IIR direct form I or II using 5 
  coefficients per bi-quad + gain term) . Input data are stored in vector x. 
  Filter output samples are stored in vector r. The filter calculates N output 
  samples using SOS and G matrices.
  Filters are able to process data in following formats:
  -  real (just array of samples)
  -  2-way or complex (interleaved real/imaginary samples)
  -  3-way (stream of interleaved samples from 3 channels)
  The same coefficients are used for filtering of multiple channels or 
  real/imaginary parts and they are processed independently. 
  The same format have to be used both for input and output streams.
  NOTES:
  1. Bi-quad coefficients may be derived from standard SOS and G matrices
     generated by MATLAB. However, typically biquad stages have big peaks 
     in their step response which may cause undesirable overflows at the 
     intermediate outputs. To avoid that the additional scale factors 
     coef_g[M] may be applied. These per-section scale factors may require 
     some tuning to find a compromise between quantization noise and possible
     overflows. Output of the last section is directed to an additional 
     multiplier, with the gain factor being a power of two, either negative 
     or non-negative. It is specified through the total gain shift amount 
     parameter gain of each filter initialization function.
  2. 16x16 filters may suffer more from accumulation of the roundoff errors, 
     so filters should be properly designed to match noise requirements

  Precision: 
  16x16  16-bit data, 16-bit coefficients, 16-bit intermediate stage outputs 
         (DF I, DF II), real data
  16x16  16-bit data, 16-bit coefficients, 16-bit intermediate stage outputs 
         (DF I, DF II), 3-way data
  24x24  32-bit data, 24-bit coefficients, 32-bit intermediate stage outputs 
         (DF I, DF II), real data
  32x16  32-bit data, 16-bit coefficients, 32-bit intermediate stage outputs 
         (DF I, DF II), real data
  32x16  32-bit data, 16-bit coefficients, 32-bit intermediate stage outputs 
         (DF I, DF II), 3-way data
  32x32  32-bit data, 32-bit coefficients, 32-bit intermediate stage outputs 
         (DF I, DF II)
  32x32  32-bit data, 32-bit coefficients, 32-bit intermediate stage outputs 
         (DF I, DF II) 3-way data
  f      floating point (DF I, DF II and DF IIt)
  f      floating point (DF I), 2-way (complex) data
  f      floating point (DF I, DF II) 3-way data

   ----------------+--------------------------------
   Functon         | Scratch memory, bytes
   ----------------+--------------------------------
   bqriir16x16_df1 | BQRIIR16X16_DF1_SCRATCH_SIZE(M)
   bqriir16x16_df2 | BQRIIR16X16_DF2_SCRATCH_SIZE(M)
   bq3iir16x16_df1 | BQ3IIR16X16_DF1_SCRATCH_SIZE(M)
   bq3iir16x16_df2 | BQ3IIR16X16_DF2_SCRATCH_SIZE(M)
   bqriir24x24_df1 | BQRIIR24X24_DF1_SCRATCH_SIZE(M)
   bqriir24x24_df2 | BQRIIR24X24_DF2_SCRATCH_SIZE(M)
   bqriir32x16_df1 | BQRIIR32X16_DF1_SCRATCH_SIZE(M)
   bqriir32x16_df2 | BQRIIR32X16_DF2_SCRATCH_SIZE(M)
   bq3iir32x16_df1 | BQ3IIR32X16_DF1_SCRATCH_SIZE(M)
   bq3iir32x16_df2 | BQ3IIR32X16_DF2_SCRATCH_SIZE(M)
   bqriir32x32_df1 | BQRIIR32X32_DF1_SCRATCH_SIZE(M)
   bqriir32x32_df2 | BQRIIR32X32_DF2_SCRATCH_SIZE(M)
   bq3iir32x32_df1 | BQ3IIR32X32_DF1_SCRATCH_SIZE(M)
   bq3iir32x32_df2 | BQ3IIR32X32_DF2_SCRATCH_SIZE(M)
   ----------------+--------------------------------
  Input:
  N             length of input sample block. For 3-way functions (bq3iirxxx), 
                N is a number of triplets
  M             number of bi-quad sections
  s[]           scratch memory area (for fixed-point functions only), Minimum 
                number of bytes depends on selected filter structure and 
                precision. see table above .If a particular macro returns zero, 
                then the corresponding IIR doesn't require a scratch area and 
                parameter s may hold zero

  coef_sos[M*5] filter coefficients stored in blocks of 5 numbers: 
                b0 b1 b2 a1 a2. 
                For fixed-point funcions, fixed point format of filter 
                coefficients is Q1.14 for 32x16, or Q1.30 for 32x16 and 
                24x24 (in the latter case 8 LSBs are actually ignored). 
  coef_g[M]     scale factor for each section, Q15 (for fixed-point 
                functions only). Please note that 24x24 DFI implementation 
                internally truncates scale factors to Q7 values.
  gain          total gain shift amount applied to output signal of the
                last section, -48..15
  x[N]          input samples, Q31, Q15 or floating point. For 3-way functions 
                (bq3iirxxx), N is a number of triplets, so array size should be 
                3*N.

  Output:
  r[N]          output data, Q31, Q15 or floating point. For 3-way functions 
                (bq3iirxxx), N is a number of triplets, so array size should be 
                3*N.

  Restriction:
  x,r,s,coef_g,coef_sos should not overlap
  N   - must be a multiple of 2
  s[] - whenever supplied must be aligned on an 8-bytes boundary

-------------------------------------------------------------------------*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
/* Common utility and macros declarations. */
#include "common.h"

/* Instance pointer validation number. */
#define MAGIC     0x5429AD7E

/* Reserve memory for alignment. */
#define ALIGNED_SIZE( size, align ) \
      ( (size_t)(size) + (align) - 1 )

/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) \
      (void*)( ( (uintptr_t)(addr) + ( (align) - 1 ) ) & ~( (align) - 1 ) )

#define sz_i16 sizeof(int16_t)
#define sz_i32 sizeof(int32_t)

/* Filter instance structure. */
typedef struct tag_bq3iir32x16_df2_t
{
  uint32_t        magic;  // Instance pointer validation number
  int             M;      // Number of sections
  int16_t         gain;   // Gain shift amount applied after the last section
  const int16_t * coef;   // Num/den coefs (Q14) and gain (Q15) for each biquad
  int32_t *       state;  // 2 state elements per section, Q31

} bq3iir32x16_df2_t, *bq3iir32x16_df2_ptr_t;

// Determine the memory area size for a filter instance.
size_t bq3iir32x16_df2_alloc( int M )
{
  ASSERT( M >= 0 );

  return ( ALIGNED_SIZE( sizeof(bq3iir32x16_df2_t), 4 )
           + // 2 state elements for each of M DFII sections.
           ALIGNED_SIZE( 3*2*M*sz_i32, 2*sz_i32 )
           + // 6+2 coefficients for each of M sections
           ALIGNED_SIZE( 8*M*sz_i16, 4*sz_i16 ) );

} // bq3iir32x16_df2_alloc()

// Given a memory area of sufficient size, initialize a filter instance and 
// return a handle to it.
bq3iir32x16_df2_handle_t bq3iir32x16_df2_init( void * objmem, int M,
                                               const int16_t * coef_sos,
                                               const int16_t * coef_g,
                                               int16_t         gain )
{
  bq3iir32x16_df2_ptr_t bq3iir;

  int32_t * sectState;
  int16_t * sectCoef;
  void *    ptr;

  int m;

  ASSERT( objmem &&  M >= 0 && coef_sos && coef_g );

  ASSERT( -48 <= gain && gain <= 15 );

  //
  // Partition the memory block
  //

  ptr = objmem;

  bq3iir    = (bq3iir32x16_df2_ptr_t)ALIGNED_ADDR( ptr, 4 );
  ptr       = bq3iir + 1;
  sectState = (int32_t *)ALIGNED_ADDR( ptr, 2*sz_i32 );
  ptr       = sectState + 3*2*M;
  sectCoef  = (int16_t *)ALIGNED_ADDR( ptr, 4*sz_i16 );
  ptr       = sectCoef + 8*M;

  ASSERT( (int8_t*)ptr - (int8_t*)objmem <= (int)bq3iir32x16_df2_alloc(M ) );

  //
  // Copy filter coefficients for M sections, zero the state elements.
  //

  for ( m=0; m<M; m++ )
  {
    sectCoef[8*m+0] = coef_g[m];       // g  [Q15]
    sectCoef[8*m+1] = coef_g[m];       // g  [Q15]
    sectCoef[8*m+2] = coef_sos[5*m+0]; // b0 [Q14]
    sectCoef[8*m+3] = coef_sos[5*m+0]; // b0 [Q14]
    sectCoef[8*m+4] = coef_sos[5*m+1]; // b1 [Q14]
    sectCoef[8*m+5] = coef_sos[5*m+2]; // b2 [Q14]
    sectCoef[8*m+6] = coef_sos[5*m+3]; // a1 [Q14]
    sectCoef[8*m+7] = coef_sos[5*m+4]; // a2 [Q14]
  }
  for ( m=0; m<3*2*M; m++ )
  {
    sectState[m] = 0;
  }

  //
  // Initialize the filter instance.
  //

  bq3iir->magic = MAGIC;
  bq3iir->M     = M;
  bq3iir->gain  = gain;
  bq3iir->coef  = sectCoef;
  bq3iir->state = sectState;

  return (bq3iir);

} // bq3iir32x16_df2_init()

// Process data. The filter instance is identified with a handle returned by
// the initializing function.
void bq3iir32x16_df2( bq3iir32x16_df2_handle_t _bq3iir,
                      void    * restrict       s,
                      int32_t * restrict       r,
                const int32_t *                x, int N )
{
  bq3iir32x16_df2_ptr_t bq3iir = (bq3iir32x16_df2_ptr_t)_bq3iir;

  const ae_int32x2 * restrict X;
        ae_f32x2   * restrict R;
        ae_f32x2   *          S;
  const ae_f16x4   *          C;
  ae_int32   X0, X1, X2;
  ae_f32x2   x0, x1, x2;
  ae_f32x2   y0, y1, y2;
  ae_f32x2   d0_0, d0_1, d0_2;
  ae_f32x2   d1d2_0, d1d2_1, d1d2_2;
  ae_f64     q0, q1, q2;
  ae_f16x4   cf0, cf1;
  int32_t gain;

  int n, m;
  int M;

  ASSERT( bq3iir && bq3iir->magic == MAGIC && r && x );
  ASSERT( !( N & 1 ) );

  if(N<=0) return;

  M = bq3iir->M;
  gain = bq3iir->gain;

  // Set the pointer to the filter state elements.
  S = (ae_f32x2 *)bq3iir->state;
  // Set the pointer to the section coefficients.
  C = (const ae_f16x4 *)bq3iir->coef;
  // When processing the first biquad section, use x[N] for the data input.
  X = (const ae_int32x2 *)(x);

  //----------------------------------------------------------------------------
  // Perform data block processing for each section but the last one.
  //
  for ( m=0; m<M-1; m++ )
  {
    // Output data of all biquads are stored to the output array.
    R = (ae_f32x2 *)(r);

    //
    // Load m-th section's SOS/g coefficients.
    //
                                   //          3      2      1      0
    ae_f16x4_loadip( cf0, C, +8 ); // cf0 <-  g_q15  g_q15 b0_q14 b0_q14
    ae_f16x4_loadip( cf1, C, +8 ); // cf1 <- b1_q14 b2_q14 a1_q14 a2_q14

    //
    // Load m-th section's states for 3 streams.
    // Q31
    d1d2_0 = ae_f32x2_loadi( S, +0 );
    d1d2_1 = ae_f32x2_loadi( S, +8 );
    d1d2_2 = ae_f32x2_loadi( S, +16);

    //
    // Pass the block of input samples through the section. n-th sample at the
    // output of a biquad section r[n] and section state elements (d1,d2) update
    // are given by the following formulae:
    //   d0   = g*x[n] - a1*d1 - a2*d2
    //   r[n] = b0*d0 + b1*d1 + b2*d2;
    //   d2 = d1; d1 = d0;
    // All 3 streams are computed simultaneously.
    //
    __Pragma("loop_count min=2, factor=2")
    for ( n=0; n<N; n++ )
    {
      // Load 3 input samples of 3 streams.
      // Q31
      ae_int32_loadip(X0,castxcc(const ae_int32,X),sz_i32);
      ae_int32_loadip(X1,castxcc(const ae_int32,X),sz_i32);
      ae_int32_loadip(X2,castxcc(const ae_int32,X),sz_i32);
      x0 = AE_MOVF32X2_FROMINT32(X0);
      x1 = AE_MOVF32X2_FROMINT32(X1);
      x2 = AE_MOVF32X2_FROMINT32(X2);

      // q = g*x[n];
      // Q17.46 <- Q31*Q15
      q0 = AE_MUL32X16_L3( x0, cf0 );
      q1 = AE_MUL32X16_L3( x1, cf0 );
      q2 = AE_MUL32X16_L3( x2, cf0 );
      // q = q - d1*a1 - d2*a2;
      // Q17.46 <- Q17.46 - [ Q31*Q14 + 1 ] - [ Q31*Q14 + 1 ]
      AE_MULSSFD32X16_H1_L0( q0, d1d2_0, cf1 );
      AE_MULSSFD32X16_H1_L0( q1, d1d2_1, cf1 );
      AE_MULSSFD32X16_H1_L0( q2, d1d2_2, cf1 );
      // d0 = q;
      // Q31 <- Q17.46 + 1 - 16 w/ asym. rounding and saturation
      d0_0 = AE_MOVF32X2_FROMF64(AE_PKSR32_0( q0, 1 ));
      d0_1 = AE_MOVF32X2_FROMF64(AE_PKSR32_0( q1, 1 ));
      d0_2 = AE_MOVF32X2_FROMF64(AE_PKSR32_0( q2, 1 ));
      // q = d0*b0;
      // Q17.46 <- Q31*Q14 + 1
      q0 = AE_MULF32X16_L0( d0_0, cf0 );
      q1 = AE_MULF32X16_L0( d0_1, cf0 );
      q2 = AE_MULF32X16_L0( d0_2, cf0 );
      // q = q + d1*b1 + d2*b2;
      // Q17.46 <- Q17.46 + [ Q31*Q14 + 1 ] + [ Q31*Q14 + 1 ]
      AE_MULAAFD32X16_H3_L2( q0, d1d2_0, cf1 );
      AE_MULAAFD32X16_H3_L2( q1, d1d2_1, cf1 );
      AE_MULAAFD32X16_H3_L2( q2, d1d2_2, cf1 );

      // y <- q
      // Q31 <- Q17.46 + 1 - 16 w/ asym. rounding and saturation
      y0 = AE_MOVF32X2_FROMF64(AE_PKSR32_0( q0, 1 ));
      y1 = AE_MOVF32X2_FROMF64(AE_PKSR32_0( q1, 1 ));
      y2 = AE_MOVF32X2_FROMF64(AE_PKSR32_0( q2, 1 ));
      // Save the output samples.
      AE_S32_L_IP(y0,castxcc(ae_int32,R),sz_i32);
      AE_S32_L_IP(y1,castxcc(ae_int32,R),sz_i32);
      AE_S32_L_IP(y2,castxcc(ae_int32,R),sz_i32);

      // Update the delay lines:
      // d2 = d1; d1 = d0.
      d1d2_0 = AE_SEL32_LH( d0_0, d1d2_0 );
      d1d2_1 = AE_SEL32_LH( d0_1, d1d2_1 );
      d1d2_2 = AE_SEL32_LH( d0_2, d1d2_2 );
    }

    //
    // Save m-th section's state.
    //

    ae_f32x2_storeip( d1d2_0, S, +8 );
    ae_f32x2_storeip( d1d2_1, S, +8 );
    ae_f32x2_storeip( d1d2_2, S, +8 );

    //
    // From now on biquads are fed with outputs of preceding sections.
    //

    X = (const ae_int32x2 *)(r);
  }

  //----------------------------------------------------------------------------
  // Pass signal through the last biquad and apply the total gain.
  //

  // Last section's outputs will be stored to the output array.
  R = (ae_f32x2 *)(r);

  //
  // Load m-th section's SOS/g coefficients.
  //
                                 //          3      2      1      0
  ae_f16x4_loadip( cf0, C, +8 ); // cf0 <-  g_q15  g_q15 b0_q14 b0_q14
  ae_f16x4_loadip( cf1, C, +8 ); // cf1 <- b1_q14 b2_q14 a1_q14 a2_q14

  //
  // Load last section's state.
  // Q31
  d1d2_0 = ae_f32x2_loadi( S, +0 );
  d1d2_1 = ae_f32x2_loadi( S, +8 );
  d1d2_2 = ae_f32x2_loadi( S, +16);
    
  //
  // Pass the block of input samples through the section. n-th sample at the
  // output of a biquad section r[n] and section state elements (d1,d2) update
  // are given by the following formulae:
  //   d0   = g*x[n] - a1*d1 - a2*d2
  //   y[n] = b0*d0 + b1*d1 + b2*d2;
  //   d2 = d1; d1 = d0;
  // Final result is obtained by applying the total gain shift amount to
  // response of the last biquad:
  //   r[n] = 2^gain*y[n]
  // All 3 streams are computed simultaneously.
  //
  __Pragma("loop_count min=2, factor=2")
  for ( n=0; n<N; n++ )
  {
    // Load 3 input samples of 3 streams.
    // Q31
    ae_int32_loadip(X0,castxcc(const ae_int32,X),sz_i32);
    ae_int32_loadip(X1,castxcc(const ae_int32,X),sz_i32);
    ae_int32_loadip(X2,castxcc(const ae_int32,X),sz_i32);
    x0 = AE_MOVF32X2_FROMINT32(X0);
    x1 = AE_MOVF32X2_FROMINT32(X1);
    x2 = AE_MOVF32X2_FROMINT32(X2);
      
    // q = g*x[n];
    // Q17.46 <- Q31*Q15
    q0 = AE_MUL32X16_L3( x0, cf0 );
    q1 = AE_MUL32X16_L3( x1, cf0 );
    q2 = AE_MUL32X16_L3( x2, cf0 );
    // q = q - d1*a1 - d2*a2;
    // Q17.46 <- Q17.46 - [ Q31*Q14 + 1 ] - [ Q31*Q14 + 1 ]
    AE_MULSSFD32X16_H1_L0( q0, d1d2_0, cf1 );
    AE_MULSSFD32X16_H1_L0( q1, d1d2_1, cf1 );
    AE_MULSSFD32X16_H1_L0( q2, d1d2_2, cf1 );
    // d0 = q;
    // Q31 <- Q17.46 + 1 - 16 w/ asym. rounding and saturation
    d0_0 = AE_MOVF32X2_FROMF64(AE_PKSR32_0( q0, 1 ));
    d0_1 = AE_MOVF32X2_FROMF64(AE_PKSR32_0( q1, 1 ));
    d0_2 = AE_MOVF32X2_FROMF64(AE_PKSR32_0( q2, 1 ));
    // q = d0*b0;
    // Q17.46 <- Q31*Q14 + 1
    q0 = AE_MULF32X16_L0( d0_0, cf0 );
    q1 = AE_MULF32X16_L0( d0_1, cf0 );
    q2 = AE_MULF32X16_L0( d0_2, cf0 );
    // q = q + d1*b1 + d2*b2;
    // Q17.46 <- Q17.46 + [ Q31*Q14 + 1 ] + [ Q31*Q14 + 1 ]
    AE_MULAAFD32X16_H3_L2( q0, d1d2_0, cf1 );
    AE_MULAAFD32X16_H3_L2( q1, d1d2_1, cf1 );
    AE_MULAAFD32X16_H3_L2( q2, d1d2_2, cf1 );

    // Apply the total gain shift
    // Q(17.47+gain) <- [ Q17.46 + 1 + gain ] w/ saturation
    q0 = AE_F64_SLAS(q0, 1+gain);
    q1 = AE_F64_SLAS(q1, 1+gain);
    q2 = AE_F64_SLAS(q2, 1+gain);

    // Format and save the output samples.
    // Q31 <- Q17.47 w/ asym. rounding and saturation
    AE_S32RA64S_IP(q0, castxcc(ae_f32, R), sz_i32);
    AE_S32RA64S_IP(q1, castxcc(ae_f32, R), sz_i32);
    AE_S32RA64S_IP(q2, castxcc(ae_f32, R), sz_i32);

    // Update the delay lines:
    // d2 = d1; d1 = d0.
    d1d2_0 = AE_SEL32_LH( d0_0, d1d2_0 );
    d1d2_1 = AE_SEL32_LH( d0_1, d1d2_1 );
    d1d2_2 = AE_SEL32_LH( d0_2, d1d2_2 );
  }

  //
  // Save last section's state.
  //

  ae_f32x2_storeip( d1d2_0, S, +8 );
  ae_f32x2_storeip( d1d2_1, S, +8 );
  ae_f32x2_storeip( d1d2_2, S, +8 );

} // bq3iir32x16_df2()
