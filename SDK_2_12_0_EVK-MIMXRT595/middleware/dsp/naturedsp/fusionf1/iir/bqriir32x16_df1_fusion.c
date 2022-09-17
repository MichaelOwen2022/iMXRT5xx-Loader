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
    Bi-quad Real Block IIR, 32x16-bit, Direct Form I
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
#define MAGIC     0x914c4a88

/* Reserve memory for alignment. */
#define ALIGNED_SIZE( size, align ) \
      ( (size_t)(size) + (align) - 1 )

/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) \
      (void*)( ( (uintptr_t)(addr) + ( (align) - 1 ) ) & ~( (align) - 1 ) )

#define sz_i16 sizeof(int16_t)
#define sz_i32 sizeof(int32_t)

/* Filter instance structure. */
typedef struct tag_bqriir32x16_df1_t
{
  uint32_t        magic; // Instance pointer validation number
  int             M;     // Number of sections
  int16_t         gain;  // Total gain shift amount for the last biquad
  const int16_t * coef;  // Num/den coefs (Q14) and gain (Q15) for each biquad
  int32_t *       state; // 4 state elements per section, Q31

} bqriir32x16_df1_t, *bqriir32x16_df1_ptr_t;

// Determine the memory area size for a filter instance.
size_t bqriir32x16_df1_alloc( int M )
{
  ASSERT( M >= 0 );

  return ( ALIGNED_SIZE( sizeof(bqriir32x16_df1_t), 4 )
           + // 4 state elements for each of M DFI sections.
           ALIGNED_SIZE( 4*M*sz_i32, 8 )
           + // 6+2 coefficients for each of M sections
           ALIGNED_SIZE( 8*M*sz_i16, 8 ) );

} // bqriir32x16_df1_alloc()

// Given a memory area of sufficient size, initialize a filter instance and 
// return a handle to it.
bqriir32x16_df1_handle_t bqriir32x16_df1_init( void * objmem, int M, 
                                               const int16_t * coef_sos,
                                               const int16_t * coef_g,
                                               int16_t         gain )
{
  bqriir32x16_df1_ptr_t bqriir;

  int32_t * sectState;
  int16_t * sectCoef;
  void *    ptr;

  int m;

  ASSERT( objmem && M >= 0 && coef_sos && coef_g );

  ASSERT( -48 <= gain && gain <= 15 );

  //
  // Partition the memory block
  //

  ptr = objmem;

  bqriir    = (bqriir32x16_df1_ptr_t)ALIGNED_ADDR( ptr, 4 );
  ptr       = bqriir + 1;
  sectState = (int32_t *)ALIGNED_ADDR( ptr, 8 );
  ptr       = sectState + 4*M;
  sectCoef  = (int16_t *)ALIGNED_ADDR( ptr, 8 );
  ptr       = sectCoef + 8*M;

  ASSERT( (int8_t*)ptr - (int8_t*)objmem <= (int)bqriir32x16_df1_alloc(M ) );

  //
  // Copy filter coefficients for M sections, zero the state elements.
  //

  for ( m=0; m<M; m++ )
  {
    int16_t g, b0, b1, b2, a1, a2;

    // Q15
    g = coef_g[m];

    // Q14
    b0 = coef_sos[5*m+0];
    b1 = coef_sos[5*m+1];
    b2 = coef_sos[5*m+2];
    a1 = coef_sos[5*m+3];
    a2 = coef_sos[5*m+4];

    // Q15
    sectCoef[8*m+0] = g;
    sectCoef[8*m+1] = g;

    // Q14
    sectCoef[8*m+2] = b2;
    sectCoef[8*m+3] = b1;
    sectCoef[8*m+4] = b0;
    sectCoef[8*m+5] = 0;
    sectCoef[8*m+6] = a2;
    sectCoef[8*m+7] = a1;

    // Q31
    sectState[4*m+0] = 0;
    sectState[4*m+1] = 0;
    sectState[4*m+2] = 0;
    sectState[4*m+3] = 0;
  }

  //
  // Initialize the filter instance.
  //

  bqriir->magic = MAGIC;
  bqriir->M     = M;
  bqriir->gain  = gain;
  bqriir->coef  = sectCoef;
  bqriir->state = sectState;

  return (bqriir);

} // bqriir32x16_df1_init()

// Process data. The filter instance is identified with a handle returned by
// the initializing function.
void bqriir32x16_df1( bqriir32x16_df1_handle_t _bqriir,
                      void    * restrict       s,
                      int32_t * restrict       r,
                const int32_t *                x, int N )
{
  bqriir32x16_df1_ptr_t bqriir = (bqriir32x16_df1_ptr_t)_bqriir;

  const ae_int32x2 *          X;
        ae_f32x2   *          S;
  const ae_f16x4   *          C;
        ae_f32x2   * restrict R;
        ae_int32   * restrict R_s;

  ae_valign X_va;

  ae_f32x2   st_x, st_y;
  ae_f16x4   cf0, cf1;
  ae_int32x2 gain;
  ae_int32x2 t0, t1;
  ae_f32x2   s0, s1;
  ae_f64     q0, q1;
  ae_int64   u0, u1;

  int n, m;
  int M;

  ASSERT( bqriir && bqriir->magic == MAGIC && r && x );

  // This function requires a scratch area.
  ASSERT( s && IS_ALIGN( s ) );

  M = bqriir->M;

  ASSERT( !( N & 1 ) );
  if(N<=0) return;

  //
  // Setup pointers.
  //

  S = (      ae_f32x2 *)bqriir->state;
  C = (const ae_f16x4 *)bqriir->coef;

  // When processing the first biquad section, use x[N] for the data input.
  X = (const ae_int32x2 *)x;

  //----------------------------------------------------------------------------
  // Perform data block processing for each section but the last one.
  //

  for ( m=0; m<M-1; m++ )
  {
    // Output data of all biquads excepting the last one are stored to the
    // scratch.
    R = (ae_f32x2 *)s;

    X_va = AE_LA64_PP( X );

    //
    // Load m-th section's SOS/g coefficients.
    //
                                   //          3     2     1      0
    ae_f16x4_loadip( cf0, C, +8 ); // cf0 <- g_q15 g_q15 b2_q14 b1_q14
    ae_f16x4_loadip( cf1, C, +8 ); // cf1 <- b0_q14  0   a2_q14 a1_q14

    //
    // Load m-th section's state.
    //

    // Q31
    st_x = ae_f32x2_loadi( S, +0 );
    st_y = ae_f32x2_loadi( S, +8 );

    //
    // Pass the block of input samples through the m-th section. Two output samples
    // are computed simultaneously:
    //   y[n  ] = g*x[n  ]*b0 + g*x[n-1]*b1 + g*x[n-2]*b2 - y[n-1]*a1 - y[n-2]*a2;
    //   y[n+1] = g*x[n+1]*b0 + g*x[n  ]*b1 + g*x[n-1]*b2 - y[n  ]*a1 - y[n-1]*a2;
    //

    for ( n=0; n<N; n+=2 )
    {
      // Q31
      AE_LA32X2_IP( t0, X_va, X );

      s0 = ( t0 );

      // g*x[n]; g*x[n+1]
      // Q31 <- Q31*Q15 - 15 w/ rounding
      s1 = AE_MULFP32X16X2RAS_H( s0, cf0 );

      // q0 = g*x[n-2]*b2 + g*x[n-1]*b1; q1 = g*x[n-1]*b2 + g*x[n]*b1
      // Q17.46 <- [ Q31*Q14 + 1 ] + [ Q31*Q14 + 1 ]
      AE_MULFD32X16X2_FIR_HL( q0, q1, st_x, s1, cf0 );
      // q0 += g*x[n]*b0; q1 += g*x[n+1]*b0
      // Q17.46 <- Q17.46 + [ Q31*Q14 + 1 ] + [ Q31*Q14 + 1 ]
      AE_MULAFD32X16X2_FIR_HH( q0, q1, s1, s1, cf1 );

      // q0 -= y[n-2]*a2 + y[n-1]*a1;
      // Q17.46 <- Q17.46 + [ Q31*Q15 + 1 ] + [ Q31*Q15 + 1 ]
      AE_MULSSFD32X16_H1_L0( q0, st_y, cf1 );

      // Update the feedback delay line by one step.
      // y[n] <- q0
      // st_y <- y[n-1] y[n]
      // Q31 <- Q17.46 + 1 - 16 w/ rounding and saturation
      AE_PKSR32( st_y, q0, 1 );

      // q1 -= y[n-1]*a2 + y[n]*a1
      // Q17.46 <- Q17.46 + [ Q31*Q15 + 1 ] + [ Q31*Q15 + 1 ]
      AE_MULSSFD32X16_H1_L0( q1, st_y, cf1 );

      // Update the feedback delay line by one step.
      // y[n+1] <- q1
      // st_y <- y[n] y[n+1]
      // Q31 <- Q17.46 + 1 - 16 w/ rounding and saturation
      AE_PKSR32( st_y, q1, 1 );

      // Save two 32-bit outputs y[n], y[n+1] to the scratch; Q31
      ae_f32x2_storeip( st_y, R, +8 );

      // Update the feedforward delay line by two steps.
      // st_x <- g*x[n] g*x[n+1]
      st_x = s1;
    }

    //
    // Save m-th section's state.
    //

    ae_f32x2_storeip( st_x, S, +8 );
    ae_f32x2_storeip( st_y, S, +8 );

    //
    // From now on biquads are fed with outputs of preceding sections.
    //

    X = (const ae_int32x2 *)s;

  #ifdef COMPILER_XTENSA
    #pragma flush_memory
  #endif
  }

  //----------------------------------------------------------------------------
  // Pass signal through the last biquad and apply the total gain.
  //

  // Last section's outputs will be stored to the output array.
  R_s = (ae_int32 *)r;

  X_va = AE_LA64_PP( X );

  //
  // Load last section's SOS/g coefficients.
  //
                                 //          3     2     1      0
  ae_f16x4_loadip( cf0, C, +8 ); // cf0 <- g_q15 g_q15 b2_q14 b1_q14
  ae_f16x4_loadip( cf1, C, +8 ); // cf1 <- b0_q14  0   a2_q14 a1_q14

  //
  // Load last section's state.
  //

  // Q31
  st_x = ae_f32x2_loadi( S, +0 );
  st_y = ae_f32x2_loadi( S, +8 );

  //
  // Load the total gain coefficient.
  //

  // Q15.24 <- Q7.8 + 16
  gain = AE_MOVDA32X2( 0, (unsigned)bqriir->gain << 16 );

  //
  // Pass the block of input samples through the last section. Two output samples
  // are computed simultaneously:
  //   y[n  ] = g*x[n  ]*b0 + g*x[n-1]*b1 + g*x[n-2]*b2 - y[n-1]*a1 - y[n-2]*a2;
  //   y[n+1] = g*x[n+1]*b0 + g*x[n  ]*b1 + g*x[n-1]*b2 - y[n  ]*a1 - y[n-1]*a2;
  //
  // Final result is obtained by applying the total gain shift amount to
  // response of the last biquad:
  //   r[n  ] = 2^gain*y[n  ]
  //   r[n+1] = 2^gain*y[n+1]
  //

  for ( n=0; n<N; n+=2 )
  {
    // Q31
    AE_LA32X2_IP( t0, X_va, X );

    s0 = ( t0 );

    // g*x[n]; g*x[n+1]
    // Q31 <- Q31*Q15 - 15 w/ rounding
    s1 = AE_MULFP32X16X2RAS_H( s0, cf0 );

    // q0 = g*x[n-2]*b2 + g*x[n-1]*b1; q1 = g*x[n-1]*b2 + g*x[n]*b1
    // Q17.46 <- [ Q31*Q14 + 1 ] + [ Q31*Q14 + 1 ]
    AE_MULFD32X16X2_FIR_HL( q0, q1, st_x, s1, cf0 );
    // q0 += g*x[n]*b0; q1 += g*x[n+1]*b0
    // Q17.46 <- Q17.46 + [ Q31*Q14 + 1 ] + [ Q31*Q14 + 1 ]
    AE_MULAFD32X16X2_FIR_HH( q0, q1, s1, s1, cf1 );

    // q0 -= y[n-2]*a2 + y[n-1]*a1;
    // Q17.46 <- Q17.46 + [ Q31*Q15 + 1 ] + [ Q31*Q15 + 1 ]
    AE_MULSSFD32X16_H1_L0( q0, st_y, cf1 );

    // Update the feedback delay line by one step.
    // y[n] <- q0
    // st_y <- y[n-1] y[n]
    // Q31 <- Q17.46 + 1 - 16 w/ rounding and saturation
    AE_PKSR32( st_y, q0, 1 );

    // q1 -= y[n-1]*a2 + y[n]*a1
    // Q17.46 <- Q17.46 + [ Q31*Q15 + 1 ] + [ Q31*Q15 + 1 ]
    AE_MULSSFD32X16_H1_L0( q1, st_y, cf1 );

    // Update the feedback delay line by one step.
    // y[n+1] <- q1
    // st_y <- y[n] y[n+1]
    // Q31 <- Q17.46 + 1 - 16 w/ rounding and saturation
    AE_PKSR32( st_y, q1, 1 );

    u0 = ( q0 );
    u1 = ( q1 );

    // Apply the total gain shift and format outputs y[n], y[n+1]
    // Q(31+gain) <- [ Q17.46 + 17 + gain ] - 32 w/ truncation and saturation
    t1 = AE_TRUNCA32X2F64S( u1, u0, 17 + bqriir->gain );

    // Store y[n] to the output array; Q(31+gain)
    AE_S32_L_IP( t1, R_s, +4 );

    // y[n+1] <-> y[n]
    t1 = AE_SEL32_LH( t1, t1 );

    // Store y[n+1] to the output array; Q(31+gain)
    AE_S32_L_IP( t1, R_s, +4 );

    // Update the feedforward delay line by two steps.
    // st_x <- g*x[n] g*x[n+1]
    st_x = s1;
  }

  //
  // Save last section's state.
  //

  ae_f32x2_storeip( st_x, S, +8 );
  ae_f32x2_storeip( st_y, S, +8 );

} // bqriir32x16_df1()