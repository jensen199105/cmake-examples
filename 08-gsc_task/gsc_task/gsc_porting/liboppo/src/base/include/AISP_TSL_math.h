/************************************************************
  Copyright (C), AISpeechTech. Co., Ltd.

  FileName   : AISP_TSL_math.h
  Project    :
  Module     :
  Version    :
  Date       : 2019/04/28
  Author     : Chao.Xu
  Document   :
  Description:

  Function List :

  History :
  <author>      <time>            <version >        <desc>
  Chao.Xu       2019/04/28        1.00              Create

************************************************************/
#ifndef __AISP_TSL_MATH_H__
#define __AISP_TSL_MATH_H__

#ifndef __AISP_TSL_BASE_H__
#error "forbid to include this header directly"
#endif

#ifdef __KERNEL__
#include <linux/kernel.h>
#include <linux/math64.h>
#endif

#define AISP_TSL_MAX(a, b)              ((a) > (b) ? (a) : (b))
#define AISP_TSL_MIN(a, b)              ((a) < (b) ? (a) : (b))
#define AISP_TSL_ABS(a)                 ((a) < 0 ? (-(a)): (a))

#define PDIV(a,b)                       (((a)>=0) ? (((a)+((b)>>1))/(b)) : (((a)-((b)>>1))/(b)))
#ifdef __KERNEL__
#define DIV64_32(a,b)                   (div_s64((a),(b)))
#define DIV64_64(a,b)                   (div64_s64((a),(b)))
#define PDIV64(a,b)                     (((a)>=0)? DIV64_64(((S64)(a)+((S64)(b)>>1)),((S64)(b))):\
                                               DIV64_64(((S64)(a)-((S64)(b)>>1)),((S64)(b))))
#else
#define DIV64_32(a,b)                   ((a)/(b))
#define DIV64_64(a,b)                   ((a)/(b))
#define PDIV64(a,b)                     (((a)>=0)?((S64)(a)+((S64)(b)>>1))/((S64)(b)):\
                                               ((S64)(a)-((S64)(b)>>1))/((S64)(b)))
#endif

VOID AISP_TSL_add_vec_32(S32 *piSrcA, S32 *piSrcB, S32 *piDst, S32 len);
VOID AISP_TSL_sub_vec_32(S32 *piSrcA, S32 *piSrcB, S32 *piDst, S32 len);

VOID AISP_TSL_cmul_vec_32_32(S32 *piSrcA, S32 *piSrcB, S32 *piDst, S32 len, S32 shift);
VOID AISP_TSL_cmul_sum_vec_32_32(S32 *piSrcA, S32 *piSrcB, S32 *piDst, S32 len, S32 shift);
VOID AISP_TSL_cmulj_sum_vec_with_mu_32_32(S64 *piSrcA, S32 *piSrcB, S32 *piDst, S32 iMu, S32 len, S32 shift,
        S32 iMuQVal);

S32 AISP_TSL_sigmoid_xW32Q24_yW32Q24(S32 x);
U32 AISP_TSL_sqrt32(U32 x);
U32 AISP_TSL_sqrt_xW64_yW32(U64 x);

#endif
