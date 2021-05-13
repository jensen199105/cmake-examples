/* Copyright Statement:
 *
 * (C) 2020  MediaTek Inc. All rights reserved.
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. ("MediaTek") and/or its licensors.
 * Without the prior written permission of MediaTek and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 * You may only use, reproduce, modify, or distribute (as applicable) MediaTek Software
 * if you have agreed to and been bound by the applicable license agreement with
 * MediaTek ("License Agreement") and been granted explicit permission to do so within
 * the License Agreement ("Permitted User").  If you are not a Permitted User,
 * please cease any access or use of MediaTek Software immediately.
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT MEDIATEK SOFTWARE RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES
 * ARE PROVIDED TO RECEIVER ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */

#ifndef DSP_LIB_H
#define DSP_LIB_H

/**
 * @defgroup dsplib_typedef DSP Library Typedef
 * @defgroup dsplib_macro DSP Library Macro
 * @defgroup dsplib_enum DSP Library Enums
 */

#include <stdint.h>

#if defined(DSPLIB_ASSERT)
#include <assert.h>
#define ASSERT(x) assert(x)
#else
#define ASSERT(x) (void)0
#endif

#if defined(DSPLIB_LOG)
#include <stdio.h>
#include <string.h>
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define LOG(format, ...) \
do { \
       printf("%s(%d) " format, __FILENAME__, __LINE__, ##__VA_ARGS__); \
} while (0)
/* The macro shows the floating and hex of a QL format number with configurable q */
#define LOG_QL(v, q) LOG(#v "::Q%d.%d=0x%08x, " #v "::F=%f\n", 32-(q), (q), (v), Q2F_L((v), (q)))
/* The macro shows the floating and hex of a QS format number with configurable q */
#define LOG_QS(v, q) LOG(#v "::Q%d.%d=0x%hx, " #v "::F=%f\n", 16-(q), (q), (v), Q2F_S((v), (q)))
#else
#define LOG(format, ...)
#define LOG_QL(v, q)
#define LOG_QS(v, q)
#endif

/** @addtogroup dsplib_typedef
  * @{
  */
/** @brief  This defines FIXP_LL type which is long long fixed point data type used in dsp library.
 */
typedef int64_t FIXP_LL;

/** @brief  This defines the FIXP_L type which is long fixed point data type used in dsp library.
 */
typedef int32_t FIXP_L;

/** @brief  This defines the FIXP_S type which is short fixed point data type used in dsp library.
 */
typedef int16_t FIXP_S;
/**
  * @}
  */

/** @addtogroup dsplib_macro
  * @{
  */
/** @brief  This macro converts the fixed point number of the specified long Q format to the floating point number.
  * It is used in run time with sanity check enabled if DSPLIB_ASSERT is defined.
 */
#define Q2F_L(QNUM,Q) (ASSERT((Q) >=0 && (Q) <= 31), (float)((FIXP_L)(QNUM)/(float)(1UL << (Q))))

/** @brief  This macro converts the fixed point number of the specified short Q format to the floating point number.
  * It is used in run time with sanity check enabled if DSPLIB_ASSERT is defined.
 */
#define Q2F_S(QNUM,Q) (ASSERT((Q) >=0 && (Q) <= 15), (float)((FIXP_S)(QNUM)/(float)(1UL << (Q))))

/** @brief  This macro converts the fixed point number of the specified long Q format to the floating point number.
  * It is used to define compile time constants.
 */
#define Q2F_L_CONST(QNUM,Q) (float)((FIXP_L)(QNUM)/(float)(1UL << (Q)))

/** @brief  This macro converts the fixed point number of the specified short Q format to the floating point number.
  * It is used to define compile time constants.
 */
#define Q2F_S_CONST(QNUM,Q) (float)((FIXP_S)(QNUM)/(float)(1UL << (Q)))

/** @brief  This defines the maximum long fixed point number.
 */
#define QMAX_L 0x7fffffff

/** @brief  This defines the minimum long fixed point number.
 */
#define QMIN_L 0x80000000

/** @brief  This defines the maximum floating point value corresponding to the specified long Q format.
 */
#define FMAX_L(Q) Q2F_L_CONST(QMAX_L, Q)

/** @brief  This defines the minimum floating point value corresponding to the specified long Q format.
 */
#define FMIN_L(Q) Q2F_L_CONST(QMIN_L, Q)

/** @brief  This defines the maximum short fixed point number.
 */
#define QMAX_S 0x7fff

/** @brief  This defines the minimum short fixed point number.
 */
#define QMIN_S 0x8000

/** @brief  This defines the maximum floating point value corresponding to the specified short Q format.
 */
#define FMAX_S(Q) Q2F_S_CONST(QMAX_S, Q)

/** @brief  This defines the minimum floating point value corresponding to the specified short Q format.
 */
#define FMIN_S(Q) Q2F_S_CONST(QMIN_S, Q)

/** @brief  This macro converts the floating point number to the specified long Q format fixed point number.
  * It is used in run time with sanity check enabled if DSPLIB_ASSERT is defined.
 */
#define F2Q_L(FNUM,Q) \
        (ASSERT((Q) >=0 && (Q) <= 31), \
          ((FNUM) >= FMAX_L(Q)) ? QMAX_L : \
          ((FNUM) <= FMIN_L(Q)) ? QMIN_L : \
          ((FIXP_L)((FNUM) * (float)(1UL << (Q)) + ((FNUM) > 0 ? 0.5f : -0.5f))))

/** @brief  This macro converts the floating point number to the specified short Q format fixed point number.
  * It is used in run time with sanity check enabled if DSPLIB_ASSERT is defined.
 */
#define F2Q_S(FNUM,Q) \
        (ASSERT((Q) >=0 && (Q) <= 15), \
          ((FNUM) >= FMAX_S(Q)) ? QMAX_S : \
          ((FNUM) <= FMIN_S(Q)) ? QMIN_S : \
          ((FIXP_S)((FNUM) * (float)(1UL << (Q)) + ((FNUM) > 0 ? 0.5f : -0.5f))))

/** @brief  This macro converts the floating point number to the specified long Q format fixed point number.
  * It is used to define compile time constants.
 */
#define F2Q_L_CONST(FNUM,Q) \
         (((FNUM) >= FMAX_L(Q)) ? QMAX_L : \
          ((FNUM) <= FMIN_L(Q)) ? QMIN_L : \
          ((FIXP_L)((FNUM) * (float)(1UL << (Q)) + ((FNUM) > 0 ? 0.5f : -0.5f))))

/** @brief  This macro converts the floating point number to the specified short Q format fixed point number.
  * It is used to define compile time constants.
 */
#define F2Q_S_CONST(FNUM,Q) \
         (((FNUM) >= FMAX_S(Q)) ? QMAX_S : \
          ((FNUM) <= FMIN_S(Q)) ? QMIN_S : \
          ((FIXP_S)((FNUM) * (float)(1UL << (Q)) + ((FNUM) > 0 ? 0.5f : -0.5f))))
/**
  * @}
  */

/** @addtogroup dsplib_enum
  * @{
  */
/* mrv math function status */
typedef enum {
    MRV_STATUS_OK = 0,           /**< No error occurred during the function call */
    MRV_STATUS_ARG_ERROR,        /**< An input argument error occurred during the function call */
    MRV_STATUS_LOGBASE_ERROR,    /**< A log base error occurred during the log function call */
    MRV_STATUS_DIVIDE_BY_ZERO,   /**< A divide by zero occurred during the ldiv function call */
} mrv_status;

/* log function base */
typedef enum {
    LOG_BASE_2 = 0,              /**< Call log function with log base 2 */
    LOG_BASE_e,                  /**< Call log function with log base e */
    LOG_BASE_reserved            /**< Log base reserved for future extension */
} mrv_log_base;
/**
  * @}
  */


/* Profile API */
void profile_on(void);
void profile_off(void);
void profile_end(void);
int64_t read_cycle(void);
void reset_cycle(void);
void system_init(void);

#include "dsp_lib_math.h"

#endif // DSP_LIB_H
