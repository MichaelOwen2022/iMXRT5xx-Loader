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
    Bi-quad Real Block IIR, 16x16-bit, Direct Form I
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
#define MAGIC     0x6A012898

/* Reserve memory for alignment. */
#define ALIGNED_SIZE( size, align ) \
      ( (size_t)(size) + (align) - 1 )

/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) \
      (void*)( ( (uintptr_t)(addr) + ( (align) - 1 ) ) & ~( (align) - 1 ) )

#define sz_i16 sizeof(int16_t)

/* Filter instance structure. */
typedef struct tag_bqriir16x16_df1_t
{
  uint32_t        magic; // Instance pointer validation number
  int             M;     // Number of sections
  int16_t         gain;  // Total gain shift amount for the last biquad
  const int16_t * coef;  // Num/den coefs (Q14) and gain (Q15) for each biquad
  int16_t *       state; // 4 state elements per section, Q15

} bqriir16x16_df1_t, *bqriir16x16_df1_ptr_t;

// Determine the memory area size for a filter instance.
size_t bqriir16x16_df1_alloc( int M )
{
  ASSERT( M >= 0 );

  return ( ALIGNED_SIZE( sizeof(bqriir16x16_df1_t), 4 )
           + // 4 state elements for each of M DFI sections.
           ALIGNED_SIZE( 4*M*sz_i16, 4*sz_i16 )
           + // 6+2 coefficients for each of M sections
           ALIGNED_SIZE( 8*M*sz_i16, 4*sz_i16 ) );

} // bqriir16x16_df1_alloc()

// Given a memory area of sufficient size, initialize a filter instance and 
// return a handle to it.
bqriir16x16_df1_handle_t bqriir16x16_df1_init( void * objmem,  int M, 
                                               const int16_t * coef_sos,
                                               const int16_t * coef_g,
                                               int16_t         gain )
{
  bqriir16x16_df1_ptr_t bqriir;

  int16_t * sectState;
  int16_t * sectCoef;
  void *    ptr;

  int m;

  ASSERT( objmem &&  M >= 0 && coef_sos && coef_g );

  ASSERT( -48 <= gain && gain <= 15 );

  //
  // Partition the memory block
  //

  ptr = objmem;

  bqriir    = (bqriir16x16_df1_ptr_t)ALIGNED_ADDR( ptr, 4 );
  ptr       = bqriir + 1;
  sectState = (int16_t *)ALIGNED_ADDR( ptr, 4*sz_i16 );
  ptr       = sectState + 4*M;
  sectCoef  = (int16_t *)ALIGNED_ADDR( ptr, 4*sz_i16 );
  ptr       = sectCoef + 8*M;

  ASSERT( (int8_t*)ptr - (int8_t*)objmem <= (int)bqriir16x16_df1_alloc(M ) );

  //
  // Copy filter coefficients for M sections, zero the state elements.
  //

#if (XCHAL_HAVE_FUSION_16BIT_BASEBAND)
  for ( m=0; m<M; m++ )
  {
    sectCoef[8*m+0] = coef_g[m];                        //   g, Q15
    sectCoef[8*m+1] = 0;
    sectCoef[8*m+2] = AE_NEG16S_scalar(coef_sos[5*m+3]);// -a1, Q14
    sectCoef[8*m+3] = AE_NEG16S_scalar(coef_sos[5*m+4]);// -a2, Q14
    sectCoef[8*m+4] = coef_sos[5*m+0];                  //  b0, Q14
    sectCoef[8*m+5] = coef_sos[5*m+1];                  //  b1, Q14
    sectCoef[8*m+6] = coef_sos[5*m+2];                  //  b2, Q14
    sectCoef[8*m+7] = 0;

    sectState[4*m+0] = 0;
    sectState[4*m+1] = 0;
    sectState[4*m+2] = 0;
    sectState[4*m+3] = 0;
  }
#else
  for ( m=0; m<M; m++ )
  {
    sectCoef[8*m+0] = coef_g[m];       // g  [Q15]
    sectCoef[8*m+1] = coef_g[m];       // g  [Q15]
    sectCoef[8*m+2] = coef_sos[5*m+1]; // b1 [Q14]
    sectCoef[8*m+3] = coef_sos[5*m+3]; // a1 [Q14]
    sectCoef[8*m+4] = coef_sos[5*m+0]; // b0 [Q14]
    sectCoef[8*m+5] = coef_sos[5*m+1]; // b1 [Q14]
    sectCoef[8*m+6] = coef_sos[5*m+2]; // b2 [Q14]
    sectCoef[8*m+7] = coef_sos[5*m+4]; // a2 [Q14]

    sectState[4*m+0] = 0;
    sectState[4*m+1] = 0;
    sectState[4*m+2] = 0;
    sectState[4*m+3] = 0;
  }
#endif

  //
  // Initialize the filter instance.
  //

  bqriir->magic = MAGIC;
  bqriir->M     = M;
  bqriir->gain  = gain;
  bqriir->coef  = sectCoef;
  bqriir->state = sectState;

  return (bqriir);

} // bqriir16x16_df1_init()

#if (XCHAL_HAVE_FUSION_16BIT_BASEBAND)
// Process data. The filter instance is identified with a handle returned by
// the initializing function.
void bqriir16x16_df1( bqriir16x16_df1_handle_t _bqriir,
                      void    * restrict       s,
                      int16_t * restrict       r,
                const int16_t *                x, int N )
{
  bqriir16x16_df1_ptr_t bqriir = (bqriir16x16_df1_ptr_t)_bqriir;

  const int16_t    * restrict X;
        ae_int16x4 * restrict R;
        ae_int16x4 * restrict St;
  const ae_int16x4 * restrict Cf;

  int16_t    x0, x1;
  ae_int16x4 inx;
  ae_int16x4 y0, y1;
  ae_int32x2 ty0, ty1;
  ae_int64   acc0, acc1;
  ae_int16x4 sx01sr01, sx01, sr01;
  ae_int16x4 cf0, cf1, _0b0b1b2, b0b1b2_0, a1a2_00;
  ae_int16x4 _0, g;
  int M, gain;
  int m, n;

  ASSERT( bqriir && bqriir->magic == MAGIC && r && x );
  NASSERT(N%2==0);
  if(N<=0) return;
  M = bqriir->M;

  St = (ae_int16x4 *)bqriir->state;
  Cf = (ae_int16x4 *)bqriir->coef;
  X = x;
  gain = bqriir->gain;
  _0 = AE_ZERO16();

  // Perform data block processing for each section. Use the output array r[N]
  // for temporal storage of inter-section signal.
  for ( m=0; m<M-1; m++ )
  {
    //
    // Load 16-bit section coefficients.
    //
    ae_int16x4_loadip(cf0, Cf, 4*sz_i16);
    ae_int16x4_loadip(cf1, Cf, 4*sz_i16);
    g = AE_SEL16_7632(cf0, cf0);
    b0b1b2_0 = cf1;
    _0b0b1b2 = AE_SEL16_4321(cf1, cf1);
    a1a2_00  = AE_SEL16_5432(cf0, _0);

    //
    // Load 16-bit section's state elements.
    //
    sx01sr01 = ae_int16x4_loadi(St, 0);
    sx01 = AE_SEL16_7632(sx01sr01, _0);
    sr01 = AE_SEL16_5432(sx01sr01, _0);

    R = (ae_int16x4 *)r;

    //
    // Pass the block of input samples through the section. n-th sample at the
    // output of a biquad section:
    //   r[n] = g*x[n]*b0 + g*x[n-1]*b1 + g*x[n-2]*b2 - r[n-1]*a1 - r[n-2]*a2
    //
    __Pragma("loop_count min=1")
    for ( n=0; n<(N>>1); n++ )
    {
      // Load 2 input samples.
      x0 = *X++;
      x1 = *X++;
      inx = AE_MOVDA16X2(x1, x0);
      // inx <- g*x[n+1] g*x[n]
      // Q15 <- Q15*Q15 - 15 w/ rounding. No saturation is necessary.
      inx = AE_MULFC16RAS_L(inx, g);

      sx01 = AE_SEL16_5432(inx, sx01);
      // y[n] = g*x[n]*b0 + g*x[n-1]*b1 + g*x[n-2]*b2
      // y[n] = y[n] - r[n-1]*a1 - r[n-2]*a2
      // Q29 <- Q15*Q14
      acc0 = AE_MULZAAAAQ16(sx01, _0b0b1b2);
      AE_MULAAAAQ16(acc0, sr01, a1a2_00);
      // y0 <- y[n]
      // Q15 <- Q29 - 14 w/ asym. rounding and saturation
      ty0 = AE_MOVINT32X2_FROMF64(acc0);
      ty0 = AE_SLAI32S(ty0, 2);
      y0 = AE_ROUND16X4F32SASYM(ty0, ty0);
      // Update the section's state elements.
      sr01 = AE_SEL16_4321(y0, sr01);
      // y[n+1] = g*x[n+1]*b0 + g*x[n]*b1 + g*x[n-1]*b2
      // y[n+1] = y[n+1] - r[n]*a1 - r[n-1]*a2
      // Q29 <- Q15*Q14
      acc1 = AE_MULZAAAAQ16(sx01, b0b1b2_0);
      AE_MULAAAAQ16(acc1, sr01, a1a2_00);
      // y1 <- y[n+1]
      // Q15 <- Q29 - 14 w/ asym. rounding and saturation
      ty1 = AE_MOVINT32X2_FROMF64(acc1);
      ty1 = AE_SLAI32S(ty1, 2);
      y1 = AE_ROUND16X4F32SASYM(ty1, ty1);
      // Update the section's state elements.
      sr01 = AE_SEL16_4321(y1, sr01);
      // Store 2 output samples.
      // Q15
      AE_S16_0_IP(y0, castxcc(ae_int16, R), sz_i16);
      AE_S16_0_IP(y1, castxcc(ae_int16, R), sz_i16);
    }
    
    //
    // Save section's state elements.
    //
    sx01sr01 = AE_SEL16_7632(sx01, sr01);
    ae_f16x4_storeip(sx01sr01, St, 4*sz_i16);

    //
    // 2nd to the last sections are fed with output samples of the preceding
    // biquad.
    //

    X = r;
  }
  
  //--------------------------------------------------------------
  // Pass signal through the last biquad and apply the total gain.
  //--------------------------------------------------------------
  {
    //
    // Load 16-bit section coefficients.
    //
    ae_int16x4_loadip(cf0, Cf, 4*sz_i16);
    ae_int16x4_loadip(cf1, Cf, 4*sz_i16);
    g = AE_SEL16_7632(cf0, cf0);
    b0b1b2_0 = cf1;
    _0b0b1b2 = AE_SEL16_4321(cf1, cf1);
    a1a2_00  = AE_SEL16_5432(cf0, _0);

    //
    // Load 16-bit section's state elements.
    //
    
    sx01sr01 = ae_int16x4_loadi(St, 0);
    sx01 = AE_SEL16_7632(sx01sr01, _0);
    sr01 = AE_SEL16_5432(sx01sr01, _0);
    
    R = (ae_int16x4 *)r;

    //
    // Pass the block of input samples through the section. n-th sample at the
    // output of a biquad section:
    //   r[n] = g*x[n]*b0 + g*x[n-1]*b1 + g*x[n-2]*b2 - r[n-1]*a1 - r[n-2]*a2
    //
    __Pragma("loop_count min=1")
    for ( n=0; n<(N>>1); n++ )
    {
      // Load 2 input samples.
      x0 = *X++;
      x1 = *X++;
      inx = AE_MOVDA16X2(x1, x0);
      // inx <- g*x[n+1] g*x[n]
      // Q15 <- Q15*Q15 - 15 w/ rounding. No saturation is necessary.
      inx = AE_MULFC16RAS_L(inx, g);

      sx01 = AE_SEL16_5432(inx, sx01);
      // y[n] = g*x[n]*b0 + g*x[n-1]*b1 + g*x[n-2]*b2
      // y[n] = y[n] - r[n-1]*a1 - r[n-2]*a2
      // Q29 <- Q15*Q14
      acc0 = AE_MULZAAAAQ16(sx01, _0b0b1b2);
      AE_MULAAAAQ16(acc0, sr01, a1a2_00);
      // y0 <- y[n]
      // Q15 <- Q29 - 14 w/ rounding
      y0 = AE_MOVINT16X4_FROMF64(AE_PKSR32_0(acc0, 2));
      // Update the section's state elements.
      sr01 = AE_SEL16_4321(y0, sr01);
      // y[n+1] = g*x[n+1]*b0 + g*x[n]*b1 + g*x[n-1]*b2
      // y[n+1] = y[n+1] - r[n]*a1 - r[n-1]*a2
      // Q29 <- Q15*Q14
      acc1 = AE_MULZAAAAQ16(sx01, b0b1b2_0);
      AE_MULAAAAQ16(acc1, sr01, a1a2_00);
      // y1 <- y[n+1]
      // Q15 <- Q29 - 14 w/ rounding
      y1 = AE_MOVINT16X4_FROMF64(AE_PKSR32_0(acc1, 2));
      // Update the section's state elements.
      sr01 = AE_SEL16_4321(y1, sr01);
      
      // Apply the total gain shift and format outputs y[n], y[n+1]
      // Q(15+gain) <- Q(29 - 14 + gain) w/ rounding and saturation
      ty0 = AE_MOVINT32X2_FROMF64(acc0);
      ty1 = AE_MOVINT32X2_FROMF64(acc1);
      ty0 = AE_SRAA32RS(ty0, 14-gain);
      ty1 = AE_SRAA32RS(ty1, 14-gain);
      y0 = AE_SAT16X4(ty0, ty0);
      y1 = AE_SAT16X4(ty1, ty1);
      AE_S16_0_IP(y0, castxcc(ae_int16, R), sz_i16);
      AE_S16_0_IP(y1, castxcc(ae_int16, R), sz_i16);
    }
    
    //
    // Save section's state elements.
    //
    sx01sr01 = AE_SEL16_7632(sx01, sr01);
    ae_f16x4_storeip(sx01sr01, St, 4*sz_i16);
  }

} // bqriir16x16_df1()
#else
// Process data. The filter instance is identified with a handle returned by
// the initializing function.
void bqriir16x16_df1( bqriir16x16_df1_handle_t _bqriir,
                      void    * restrict       s,
                      int16_t * restrict       r,
                const int16_t *                x, int N )
{
  bqriir16x16_df1_ptr_t bqriir = (bqriir16x16_df1_ptr_t)_bqriir;

  const int16_t    * restrict X;
        ae_int16   * restrict R;
        ae_f16x4   * restrict S;
  const ae_f16x4   * restrict C;
  ae_f32x2   s0, s1;
  ae_f64     q0, q1;
  ae_f32x2   y0, y1;
  ae_f16x4   sx1sr1sx0sr0;
  ae_f32x2   sx0sr0, sx1sr1;
  ae_f16x4   cf0, cf1;
  ae_f16x4   out0, out1;
  int32_t    X0, X1;
  int32_t    gain;
  int M;
  int n, m;

  ASSERT( bqriir && bqriir->magic == MAGIC && r && x );
  ASSERT( !( N & 1 ) );
  if(N<=0) return;

  M = bqriir->M;
  gain = bqriir->gain;
  //
  // Set the pointer to the filter state elements.
  //
  S = (ae_f16x4 *)bqriir->state;
  
  // Set the pointer to the section coefficients.
  C = (const ae_f16x4 *)bqriir->coef;
  // When processing the first biquad section, use x[N] for the data input.
  X = x;

  //----------------------------------------------------------------------------
  // Perform data block processing for each section but the last one.
  //
  for ( m=0; m<M-1; m++ )
  {
    // Output data of all biquads are stored to the output array.
    R = (ae_int16 *)(r);

    //
    // Load m-th section's SOS/g coefficients.
    //
                                   //          3      2      1      0
    ae_f16x4_loadip( cf0, C, +8 ); // cf0 <-  g_q15  g_q15 b1_q14 a1_q14
    ae_f16x4_loadip( cf1, C, +8 ); // cf1 <- b0_q14 b1_q14 b2_q14 a2_q14

    //
    // Load m-th section's state.
    // Q15
    sx1sr1sx0sr0 = ae_f16x4_loadi(S, 0);
    sx0sr0 = AE_SEXT32X2D16_10( sx1sr1sx0sr0 );
    sx1sr1 = AE_SEXT32X2D16_32( sx1sr1sx0sr0 );

    //
    // Pass the block of input samples through the m-th section. Two output samples
    // are computed simultaneously:
    //   y[n  ] = g*x[n  ]*b0 + g*x[n-1]*b1 + g*x[n-2]*b2 - y[n-1]*a1 - y[n-2]*a2;
    //   y[n+1] = g*x[n+1]*b0 + g*x[n  ]*b1 + g*x[n-1]*b2 - y[n  ]*a1 - y[n-1]*a2;
    //
    __Pragma("loop_count min=1")
    for ( n=0; n<(N>>1); n++ )
    {
      // Load two input samples.
      // Q15
      X0 = (int32_t)*X++;
      X1 = (int32_t)*X++;
      s0 = AE_MOVDA32X2( X0, X1 );

      // s1 = g*x[n]; g*x[n+1]
      // Q15 <- Q15*Q15 - 15 w/ asym. rounding
      s1 = AE_MULFP32X16X2RAS_H( s0, cf0 );

      // q0 += g*x[n]*b0
      // Q30 <- Q15*Q14 + 1
      q0 = AE_MULF32X16_H3( s1, cf1 );
      // q0 = q0 + g*x[n-2]*b2 - y[n-2]*a2;
      // Q30 <- Q30 + [ Q15*Q14 + 1 ] - [ Q15*Q14 + 1 ]
      AE_MULASFD32X16_H1_L0( q0, sx1sr1, cf1 );
      // q0 = q0 + g*x[n-1]*b1 - y[n-1]*a1;
      // Q30 <- Q30 + [ Q15*Q14 + 1 ] + [ Q15*Q14 + 1 ]
      AE_MULASFD32X16_H1_L0( q0, sx0sr0, cf0 );
      
      // y0 <- y[n]
      // Q15 <- Q30 + 1 - 16 w/ asym. rounding and saturation
      y0 = AE_MOVF32X2_FROMF64(AE_PKSR32_0( q0, 1 ));

      // q1 = g*x[n]*b1 + g*x[n+1]*b0;
      // Q30 <- [ Q15*Q14 + 1 ] + [ Q15*Q14 + 1 ]
      q1 = AE_MULZAAFD32X16_H2_L3( s1, cf1 );
      // q1 = q1 + g*x[n-1]*b2 - y[n-1]*a2;
      // Q30 <- Q30 + [ Q15*Q14 + 1 ] - [ Q15*Q14 + 1 ]
      AE_MULASFD32X16_H1_L0( q1, sx0sr0, cf1 );
      // q1 -= y[n]*a1.
      // Q30 <- Q30 - [ Q15*Q14 + 1 ]
      AE_MULSF32X16_L0( q1, y0, cf0 );
      
      // y0 <- y[n+1]
      // Q15 <- Q30 + 1 - 16 w/ asym. rounding and saturation
      y1 = AE_MOVF32X2_FROMF64(AE_PKSR32_0( q1, 1 ));

      // Save the output samples.
      out0 = AE_SAT16X4(y1, y0);
      out1 = AE_MOVF16X4_FROMF32X2(AE_INTSWAP(AE_MOVF32X2_FROMF16X4(out0)));
      AE_S16_0_IP(out0, R, sz_i16);
      AE_S16_0_IP(out1, R, sz_i16);

      // Update both the feedforward and feedback delay lines by two steps.
      // sx0sr0 <- g*x[n+1] y[n+1]
      // sx1sr1 <- g*x[n  ] y[n  ]
      sx0sr0 = AE_SEL32_LL( s1, y1 );
      sx1sr1 = AE_SEL32_HL( s1, y0 );
    }

    //
    // Save m-th section's state.
    //

    sx1sr1sx0sr0 = AE_SAT16X4( sx1sr1, sx0sr0 );
    ae_f16x4_storeip( sx1sr1sx0sr0, S, +8 );

    //
    // From now on biquads are fed with outputs of preceding sections.
    //

    X = (const int16_t *)(r);
  }

  //----------------------------------------------------------------------------
  // Pass signal through the last biquad and apply the total gain.
  //

  // Last section's outputs will be stored to the output array.
  R = (ae_int16 *)(r);

  //
  // Load last section's SOS/g coefficients.
  //
                                 //          3      2      1      0
  ae_f16x4_loadip( cf0, C, +8 ); // cf0 <-  g_q15  g_q15 b1_q14 a1_q14
  ae_f16x4_loadip( cf1, C, +8 ); // cf1 <- b0_q14 b1_q14 b2_q14 a2_q14

  //
  // Load last section's state.
  // Q15
  sx1sr1sx0sr0 = ae_f16x4_loadi(S, 0);
  sx0sr0 = AE_SEXT32X2D16_10( sx1sr1sx0sr0 );
  sx1sr1 = AE_SEXT32X2D16_32( sx1sr1sx0sr0 );

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
  __Pragma("loop_count min=1")
  for ( n=0; n<(N>>1); n++ )
  {
    // Load two input samples.
    // Q15
    X0 = (int32_t)*X++;
    X1 = (int32_t)*X++;
    s0 = AE_MOVDA32X2( X0, X1 );

    // g*x[n]; g*x[n+1]
    // Q15 <- Q15*Q15 - 15 w/ asym. rounding
    s1 = AE_MULFP32X16X2RAS_H( s0, cf0 );

    // q0 += g*x[n]*b0
    // Q30 <- Q15*Q14 + 1
    q0 = AE_MULF32X16_H3( s1, cf1 );
    // q0 = q0 + g*x[n-2]*b2 - y[n-2]*a2;
    // Q30 <- Q30 + [ Q15*Q14 + 1 ] - [ Q15*Q14 + 1 ]
    AE_MULASFD32X16_H1_L0( q0, sx1sr1, cf1 );
    // q0 = q0 + g*x[n-1]*b1 - y[n-1]*a1;
    // Q30 <- Q30 + [ Q15*Q14 + 1 ] + [ Q15*Q14 + 1 ]
    AE_MULASFD32X16_H1_L0( q0, sx0sr0, cf0 );

    // y0 <- y[n]
    // Q15 <- Q30 + 1 - 16 w/ asym. rounding and saturation
    y0 = AE_MOVF32X2_FROMF64(AE_PKSR32_0( q0, 1 ));

    // q1 = g*x[n]*b1 + g*x[n+1]*b0;
    // Q30 <- [ Q15*Q14 + 1 ] + [ Q15*Q14 + 1 ]
    q1 = AE_MULZAAFD32X16_H2_L3( s1, cf1 );
    // q1 = q1 + g*x[n-1]*b2 - y[n-1]*a2;
    // Q30 <- Q30 + [ Q15*Q14 + 1 ] - [ Q15*Q14 + 1 ]
    AE_MULASFD32X16_H1_L0( q1, sx0sr0, cf1 );
    // q1 -= y[n]*a1.
    // Q30 <- Q30 - [ Q15*Q14 + 1 ]
    AE_MULSF32X16_L0( q1, y0, cf0 );

    // y1 <- y[n+1]
    // Q15 <- Q30 + 1 - 16 w/ asym. rounding and saturation
    y1 = AE_MOVF32X2_FROMF64(AE_PKSR32_0( q1, 1 ));

    // Update both the feedforward and feedback delay lines by two steps.
    // sx0sr0 <- g*x[n+1] y[n+1]
    // sx1sr1 <- g*x[n  ] y[n  ]
    sx0sr0 = AE_SEL32_LL( s1, y1 );
    sx1sr1 = AE_SEL32_HL( s1, y0 );

    // Apply the total gain shift,
    // Format and save the output samples.
    // Q(15+gain) <- [ Q30 - 15 + gain ] w/ asym. rounding and saturation
    y0 = AE_MOVF32X2_FROMF64(q0);
    y1 = AE_MOVF32X2_FROMF64(q1);
    y0 = AE_SRAA32RS(y0, 15-gain);
    y1 = AE_SRAA32RS(y1, 15-gain);
    out0 = AE_SAT16X4(y1, y0);
    out1 = AE_MOVF16X4_FROMF32X2(AE_INTSWAP(AE_MOVF32X2_FROMF16X4(out0)));

    AE_S16_0_IP(out0, R, sz_i16);
    AE_S16_0_IP(out1, R, sz_i16);
  }

  //
  // Save last section's state.
  //
  
  sx1sr1sx0sr0 = AE_SAT16X4( sx1sr1, sx0sr0 );
  ae_f16x4_storeip( sx1sr1sx0sr0, S, +8 );

} // bqriir16x16_df1()
#endif