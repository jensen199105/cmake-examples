/************************************************************
  Copyright (C), AISpeechTech. Co., Ltd.

  FileName   : AISP_TSL_mul.c
  Project    :
  Module     :
  Version    :
  Date       : 2019/07/15
  Author     : Youhai.Jiang
  Document   :
  Description:

  Function List :

  History :
  <author>      <time>            <version >        <desc>
  Youhai.Jiang  2019/07/15         1.00              Create

************************************************************/

#include "AISP_TSL_base.h"

#ifdef USE_CM4_OPTIMIZE
#include "arm_math.h"
#endif

/************************************************************
  Function   : AISP_TSL_cmul_vec_32_32()

  Description:
  Calls      :
  Called By  :
  Input      :
  Output     :
  Return     :
  Others     :

  History    :
    2019/07/15, Youhai.Jiang create

************************************************************/
VOID AISP_TSL_cmul_vec_32_32(S32 *piSrcA, S32 *piSrcB, S32 *piDst, S32 len, S32 shift)
{
    U32 blkCnt = 0;                               /* Loop counter */
    S32 a = 0;
    S32 b = 0;
    S32 c = 0;
    S32 d = 0;                           /* Temporary variables */
    U32 scalar = 1 << (shift - 1);
    /* Loop unrolling: Compute 4 outputs at a time */
    blkCnt = len >> 2U;

    while (blkCnt > 0U)
    {
        /* C[2 * i    ] = A[2 * i] * B[2 * i    ] - A[2 * i + 1] * B[2 * i + 1]. */
        /* C[2 * i + 1] = A[2 * i] * B[2 * i + 1] + A[2 * i + 1] * B[2 * i    ]. */
        a = *piSrcA++;
        b = *piSrcA++;
        c = *piSrcB++;
        d = *piSrcB++;
        /* store result in 3.x format in destination buffer. */
        *piDst++ = (S32)((((S64) a * c)  - ((S64) b * d) + scalar) >> shift);
        *piDst++ = (S32)((((S64) a * d)  + ((S64) b * c) + scalar) >> shift);
        a = *piSrcA++;
        b = *piSrcA++;
        c = *piSrcB++;
        d = *piSrcB++;
        *piDst++ = (S32)((((S64) a * c) - ((S64) b * d) + scalar) >> shift);
        *piDst++ = (S32)((((S64) a * d) + ((S64) b * c) + scalar) >> shift);
        a = *piSrcA++;
        b = *piSrcA++;
        c = *piSrcB++;
        d = *piSrcB++;
        *piDst++ = (S32)((((S64) a * c)  - ((S64) b * d) + scalar) >> shift);
        *piDst++ = (S32)((((S64) a * d)  + ((S64) b * c) + scalar) >> shift);
        a = *piSrcA++;
        b = *piSrcA++;
        c = *piSrcB++;
        d = *piSrcB++;
        *piDst++ = (S32)((((S64) a * c)  - ((S64) b * d) + scalar) >> shift);
        *piDst++ = (S32)((((S64) a * d)  + ((S64) b * c) + scalar) >> shift);
        /* Decrement loop counter */
        blkCnt--;
    }

    /* Loop unrolling: Compute remaining outputs */
    blkCnt = len & 3;

    while (blkCnt > 0U)
    {
        /* C[2 * i    ] = A[2 * i] * B[2 * i    ] - A[2 * i + 1] * B[2 * i + 1]. */
        /* C[2 * i + 1] = A[2 * i] * B[2 * i + 1] + A[2 * i + 1] * B[2 * i    ]. */
        a = *piSrcA++;
        b = *piSrcA++;
        c = *piSrcB++;
        d = *piSrcB++;
        /* store result in 3.x format in destination buffer. */
        *piDst++ = (S32)((((S64) a * c)  - ((S64) b * d) + scalar) >> shift);
        *piDst++ = (S32)((((S64) a * d)  + ((S64) b * c) + scalar) >> shift);
        /* Decrement loop counter */
        blkCnt--;
    }
}

/************************************************************
  Function   : AISP_TSL_cmul_sum_vec_32_32()

  Description:
  Calls      :
  Called By  :
  Input      :
  Output     :
  Return     :
  Others     :

  History    :
    2019/07/15, Youhai.Jiang create

************************************************************/
VOID AISP_TSL_cmul_sum_vec_32_32(S32 *piSrcA, S32 *piSrcB, S32 *piDst, S32 len, S32 shift)
{
    U32 blkCnt = 0;                               /* Loop counter */
    S32 a0 = 0;
    S32 b0 = 0;
    S32 c0 = 0;
    S32 d0 = 0;
    U32 scalar = 1 << (shift - 1);
    /* Loop unrolling: Compute 4 outputs at a time */
    blkCnt = len >> 2U;

    while (blkCnt > 0U)
    {
        a0 = *piSrcA++;
        b0 = *piSrcA++;
        c0 = *piSrcB++;
        d0 = *piSrcB++;
        *piDst++ += (((S64)a0 * c0) - ((S64)b0 * d0) + scalar) >> shift;
        *piDst++ += (((S64)a0 * d0) + ((S64)b0 * c0) + scalar) >> shift;
        a0 = *piSrcA++;
        b0 = *piSrcA++;
        c0 = *piSrcB++;
        d0 = *piSrcB++;
        *piDst++ += (((S64)a0 * c0) - ((S64)b0 * d0) + scalar) >> shift;
        *piDst++ += (((S64)a0 * d0) + ((S64)b0 * c0) + scalar) >> shift;
        a0 = *piSrcA++;
        b0 = *piSrcA++;
        c0 = *piSrcB++;
        d0 = *piSrcB++;
        *piDst++ += (((S64)a0 * c0) - ((S64)b0 * d0) + scalar) >> shift;
        *piDst++ += (((S64)a0 * d0) + ((S64)b0 * c0) + scalar) >> shift;
        a0 = *piSrcA++;
        b0 = *piSrcA++;
        c0 = *piSrcB++;
        d0 = *piSrcB++;
        *piDst++ += (((S64)a0 * c0) - ((S64)b0 * d0) + scalar) >> shift;
        *piDst++ += (((S64)a0 * d0) + ((S64)b0 * c0) + scalar) >> shift;
        /* Decrement loop counter */
        blkCnt--;
    }

    /* Loop unrolling: Compute remaining outputs */
    blkCnt = len & 3;

    while (blkCnt > 0U)
    {
        a0 = *piSrcA++;
        b0 = *piSrcA++;
        c0 = *piSrcB++;
        d0 = *piSrcB++;
        *piDst++ += (((S64)a0 * c0) - ((S64)b0 * d0) + scalar) >> shift;
        *piDst++ += (((S64)a0 * d0) + ((S64)b0 * c0) + scalar) >> shift;
        /* Decrement loop counter */
        blkCnt--;
    }
}

/************************************************************
  Function   : AISP_TSL_cmulj_sum_vec_with_mu_32_32()

  Description:
  Calls      :
  Called By  :
  Input      :
  Output     :
  Return     :
  Others     :

  History    :
    2019/08/09, Youhai.Jiang create

************************************************************/
VOID AISP_TSL_cmulj_sum_vec_with_mu_32_32(S64 *piSrcA, S32 *piSrcB, S32 *piDst, S32 iMu, S32 len, S32 shift,
        S32 iMuQVal)
{
    U32 blkCnt = 0;                             /* Loop counter */
    S32 c0 = 0;
    S32 d0 = 0;
    S64 a0 = 0;
    S64 b0 = 0;
    S64 tmpReal = 0;
    S64 tmpImg = 0;
    /* Loop unrolling: Compute 4 outputs at a time */
    blkCnt = len >> 2U;

    while (blkCnt > 0U)
    {
        a0 = *piSrcA++;
        b0 = *piSrcA++;
        c0 = *piSrcB++;
        d0 = *piSrcB++;
        tmpReal = (S64)a0 * c0 + (S64)b0 * d0;
        tmpImg  = (S64)b0 * c0 - (S64)a0 * d0;
        *piDst++ += tmpReal >= 0 ? (PSHR_POSITIVE((S64)iMu * (S32)PSHR_POSITIVE(tmpReal, shift), iMuQVal)) :
                    (PSHR_NEGTIVE((S64)iMu * (S32)PSHR_NEGTIVE(tmpReal, shift), iMuQVal));
        *piDst++ += tmpImg >= 0 ? (PSHR_POSITIVE((S64)iMu * (S32)PSHR_POSITIVE(tmpImg, shift), iMuQVal)) :
                    (PSHR_NEGTIVE((S64)iMu * (S32)PSHR_NEGTIVE(tmpImg, shift), iMuQVal));
        a0 = *piSrcA++;
        b0 = *piSrcA++;
        c0 = *piSrcB++;
        d0 = *piSrcB++;
        tmpReal = (S64)a0 * c0 + (S64)b0 * d0;
        tmpImg  = (S64)b0 * c0 - (S64)a0 * d0;
        *piDst++ += tmpReal >= 0 ? (PSHR_POSITIVE((S64)iMu * (S32)PSHR_POSITIVE(tmpReal, shift), iMuQVal)) :
                    (PSHR_NEGTIVE((S64)iMu * (S32)PSHR_NEGTIVE(tmpReal, shift), iMuQVal));
        *piDst++ += tmpImg >= 0 ? (PSHR_POSITIVE((S64)iMu * (S32)PSHR_POSITIVE(tmpImg, shift), iMuQVal)) :
                    (PSHR_NEGTIVE((S64)iMu * (S32)PSHR_NEGTIVE(tmpImg, shift), iMuQVal));
        a0 = *piSrcA++;
        b0 = *piSrcA++;
        c0 = *piSrcB++;
        d0 = *piSrcB++;
        tmpReal = (S64)a0 * c0 + (S64)b0 * d0;
        tmpImg  = (S64)b0 * c0 - (S64)a0 * d0;
        *piDst++ += tmpReal >= 0 ? (PSHR_POSITIVE((S64)iMu * (S32)PSHR_POSITIVE(tmpReal, shift), iMuQVal)) :
                    (PSHR_NEGTIVE((S64)iMu * (S32)PSHR_NEGTIVE(tmpReal, shift), iMuQVal));
        *piDst++ += tmpImg >= 0 ? (PSHR_POSITIVE((S64)iMu * (S32)PSHR_POSITIVE(tmpImg, shift), iMuQVal)) :
                    (PSHR_NEGTIVE((S64)iMu * (S32)PSHR_NEGTIVE(tmpImg, shift), iMuQVal));
        a0 = *piSrcA++;
        b0 = *piSrcA++;
        c0 = *piSrcB++;
        d0 = *piSrcB++;
        tmpReal = (S64)a0 * c0 + (S64)b0 * d0;
        tmpImg  = (S64)b0 * c0 - (S64)a0 * d0;
        *piDst++ += tmpReal >= 0 ? (PSHR_POSITIVE((S64)iMu * (S32)PSHR_POSITIVE(tmpReal, shift), iMuQVal)) :
                    (PSHR_NEGTIVE((S64)iMu * (S32)PSHR_NEGTIVE(tmpReal, shift), iMuQVal));
        *piDst++ += tmpImg >= 0 ? (PSHR_POSITIVE((S64)iMu * (S32)PSHR_POSITIVE(tmpImg, shift), iMuQVal)) :
                    (PSHR_NEGTIVE((S64)iMu * (S32)PSHR_NEGTIVE(tmpImg, shift), iMuQVal));
        /* Decrement loop counter */
        blkCnt--;
    }

    /* Loop unrolling: Compute remaining outputs */
    blkCnt = len & 3;

    while (blkCnt > 0U)
    {
        a0 = *piSrcA++;
        b0 = *piSrcA++;
        c0 = *piSrcB++;
        d0 = *piSrcB++;
        tmpReal = (S64)a0 * c0 + (S64)b0 * d0;
        tmpImg  = (S64)b0 * c0 - (S64)a0 * d0;
        *piDst++ += tmpReal >= 0 ? (PSHR_POSITIVE((S64)iMu * (S32)PSHR_POSITIVE(tmpReal, shift), iMuQVal)) :
                    (PSHR_NEGTIVE((S64)iMu * (S32)PSHR_NEGTIVE(tmpReal, shift), iMuQVal));
        *piDst++ += tmpImg >= 0 ? (PSHR_POSITIVE((S64)iMu * (S32)PSHR_POSITIVE(tmpImg, shift), iMuQVal)) :
                    (PSHR_NEGTIVE((S64)iMu * (S32)PSHR_NEGTIVE(tmpImg, shift), iMuQVal));
        /* Decrement loop counter */
        blkCnt--;
    }
}
