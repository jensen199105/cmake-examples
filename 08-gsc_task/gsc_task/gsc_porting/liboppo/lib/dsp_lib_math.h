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

#ifndef DSP_LIB_MATH_H
#define DSP_LIB_MATH_H

/**
 * @defgroup groupMath Math Functions
 * @defgroup groupVector Vector Functions
 */
/**
 * @addtogroup groupMath
 * @{
 * @defgroup sqrt Square Root
 * @defgroup log Logarithm
 * @defgroup cos Cosine
 * @defgroup sin Sine
 * @defgroup ldiv Long Integer Division
 * @} */
/**
 * @addtogroup groupVector
 * @{
 * @defgroup vector_add Vector Add
 * @defgroup vector_sub Vector Sub
 * @defgroup vector_mul Vector Mul
 * @defgroup vector_dot Vector Inner Product
 * @defgroup vector_abs Vector Abs
 * @defgroup vector_neg Vector Neg
 * @defgroup vector_offset Vector Offset
 * @defgroup vector_scale Vector Scale
 * @defgroup vector_shift Vector Shift
 * @} */

/**
 * @addtogroup sqrt
 *
 * Compute the square root of a Q format number.
 *
 * @section code_sqrt How to use the API
 * - Sample code
 * @include sqrt.c
 * - Result
 * @include sqrt_result.txt
 */
/**
 * @addtogroup sqrt
 * @{
 */
/**
 * @brief QL square root function.
 * @param[in] x input value
 * @param[out] *pOut square root of input value
 * @param[in] q q format specifier, 0 <= q <= 31, Q[32-q].[q]
 * @return MRV_STATUS_OK for non-negative input, or
 *         MRV_STATUS_ARG_ERROR for negative input
 */
mrv_status mrv_sqrt_ql(FIXP_L x, FIXP_L *pOut, int16_t q);

/**
 * @brief QS square root function.
 * @param[in] x input value
 * @param[out] *pOut square root of input value
 * @param[in] q q format specifier, 0 <= q <=15, Q[16-q].[q]
 * @return MRV_STATUS_OK for non-negative input, or
 *         MRV_STATUS_ARG_ERROR for negative input
 */
mrv_status mrv_sqrt_qs(FIXP_S x, FIXP_S *pOut, int16_t q);
/** @} */

/**
 * @addtogroup log
 *
 * Compute the log value of a Q format number.
 * It supports both log base 2 and e.
 *
 * @section code_log How to use the API
 * - Sample code
 * @include log.c
 * - Result
 * @include log_result.txt
 */
/**
 * @addtogroup log
 * @{
 */
/**
 * @brief QL log function.
 * @param[in] x input value
 * @param[out] *pOut log value of input
 * @param[in] base log base
 * @param[in] q q format specifier, 0 <= q <= 31, Q[32-q].[q]
 * @return MRV_STATUS_OK for positive input value, or
 *         MRV_STATUS_ARG_ERROR otherwise
 */
mrv_status mrv_log_ql(FIXP_L x, FIXP_L *pOut, mrv_log_base base, int16_t q);

/**
 * @brief QS log function.
 * @param[in] x input value
 * @param[out] *pOut log value of input
 * @param[in] base log base
 * @param[in] q q format specifier, 0 <= q <= 15, Q[16-q].[q]
 * @return MRV_STATUS_OK for positive input value, or
 *         MRV_STATUS_ARG_ERROR otherwise
 */
mrv_status mrv_log_qs(FIXP_S x, FIXP_S *pOut, mrv_log_base base, int16_t q);
/** @} */

/**
 * @addtogroup cos
 *
 * Compute the cosine value of a Q format number.
 *
 * @section code_cos How to use the API
 * - Sample code
 * @include cos.c
 * - Result
 * @include cos_result.txt
 */
/**
* @addtogroup cos
* @{
*/
/**
 * @brief QL cos function.
 * @param[in] x input value, scaled from radians [0, 2*pi] to [0, 1).
 * @param[in] q q format specifier, 0 <= q <= 31, Q[32-q].[q]
 * @return cos(x)
 */
FIXP_L mrv_cos_ql(FIXP_L x, int16_t q);

/**
 * @brief QS cos function.
 * @param[in] x input value, scaled from radians [0, 2*pi] to [0, 1).
 * @param[in] q q format specifier, 0 <= q <= 15, Q[16-q].[q]
 * @return cos(x)
 */
FIXP_S mrv_cos_qs(FIXP_S x, int16_t q);
/** @} */

/**
 * @addtogroup sin
 *
 * Compute the sine value of a Q format number.
 *
 * @section code_sin How to use the API
 * - Sample code
 * @include sin.c
 * - Result
 * @include sin_result.txt
 */
/**
* @addtogroup sin
* @{
*/
/**
 * @brief QL sin function.
 * @param[in] x input value, scaled from radians [0, 2*pi] to [0, 1).
 * @param[in] q q format specifier, 0 <= q <= 31, Q[32-q].[q]
 * @return sin(x)
 */
FIXP_L mrv_sin_ql(FIXP_L x, int16_t q);

/**
 * @brief QS sin function.
 * @param[in] x input value, scaled from radians [0, 2*pi] to [0, 1).
 * @param[in] q q format specifier, 0 <= q <= 15, Q[16-q].[q]
 * @return sin(x)
 */
FIXP_S mrv_sin_qs(FIXP_S x, int16_t q);
/** @} */

/**
 * @addtogroup vector_add
 *
 * Compute addition of two vectors.
 *
 * @section code_vector_add How to use the API
 * - Sample code
 * @include vector_add.c
 * - Result
 * @include vector_add_result.txt
 */
/**
 * @addtogroup vector_add
 * @{
 */
/**
 * @brief Compute addition of two QL format vectors.
 * @param[out]      *pDst points to the output vector
 * @param[in]       *pSrc1 points to the first vector
 * @param[in]       *pSrc2 points to the second vector
 * @param[in]       vectorSize size of the vector
 * @return none
 *
 * <b>Note</b>
 * \par
 * The function uses saturating arithmetic.
 * Results outside of the applicable range[0x80000000 0x7FFFFFFF] will be saturated.
 */
void mrv_add_ql(FIXP_L *pDst, FIXP_L *pSrc1, FIXP_L *pSrc2, uint32_t vectorSize);
/**
 * @brief Compute addition of two QS format vectors.
 * @param[out]      *pDst points to the output vector
 * @param[in]       *pSrc1 points to the first vector
 * @param[in]       *pSrc2 points to the second vector
 * @param[in]       vectorSize size of the vector
 * @return none
 *
 * <b>Note</b>
 * \par
 * The function uses saturating arithmetic.
 * Results outside of the applicable range[0x8000 0x7FFF] will be saturated.
 */
void mrv_add_qs(FIXP_S *pDst, FIXP_S *pSrc1, FIXP_S *pSrc2, uint32_t vectorSize);
/** @} */
/**
 * @addtogroup vector_sub
 *
 * Compute subtraction of two vectors.
 *
 * @section code_vector_sub How to use the API
 * - Sample code
 * @include vector_sub.c
 * - Result
 * @include vector_sub_result.txt
 */
/**
 * @addtogroup vector_sub
 * @{
 */
/**
 * @brief Compute subtraction of two QL format vectors.
 * @param[out]      *pDst points to the output vector
 * @param[in]       *pSrc1 points to the first vector
 * @param[in]       *pSrc2 points to the second vector
 * @param[in]       vectorSize size of the vector
 * @return none
 *
 * <b>Note</b>
 * \par
 * The function uses saturating arithmetic.
 * Results outside of the applicable range[0x80000000 0x7FFFFFFF] will be saturated.
 */
void mrv_sub_ql(FIXP_L *pDst, FIXP_L *pSrc1, FIXP_L *pSrc2, uint32_t vectorSize);
/**
 * @brief Compute subtraction of two QS format vectors.
 * @param[out]      *pDst points to the output vector
 * @param[in]       *pSrc1 points to the first vector
 * @param[in]       *pSrc2 points to the second vector
 * @param[in]       vectorSize size of the vector
 * @return none
 *
 * <b>Note</b>
 * \par
 * The function uses saturating arithmetic.
 * Results outside of the applicable range[0x8000 0x7FFF] will be saturated.
 */
void mrv_sub_qs(FIXP_S *pDst, FIXP_S *pSrc1, FIXP_S *pSrc2, uint32_t vectorSize);
/** @} */

/**
 * @addtogroup vector_offset
 *
 * Perform element-wise offset operation on a vector.
 *
 * @section code_vector_offset How to use the API
 * - Sample code
 * @include vector_offset.c
 * - Result
 * @include vector_offset_result.txt
 */
/**
 * @addtogroup vector_offset
 * @{
 */
/**
 * @brief Perform element-wise offset operation on a QL format vector.
 * @param[out]      *pDst points to the output vector
 * @param[in]       *pSrc points to the input vector
 * @param[in]       offset added offset
 * @param[in]       vectorSize size of the vector
 * @return none
 *
 * <b>Note</b>
 * \par
 * The function uses saturating arithmetic.
 * Results outside of the applicable range[0x80000000 0x7FFFFFFF] will be saturated.
 */

void mrv_offset_ql(FIXP_L *pDst, FIXP_L *pSrc, FIXP_L offset, uint32_t vectorSize);

/**
 * @brief Perform element-wise offset operation on a QS format vector.
 * @param[out]      *pDst points to the output vector
 * @param[in]       *pSrc points to the input vector
 * @param[in]       offset added offset
 * @param[in]       vectorSize size of the vector
 * @return none
 *
 * <b>Note</b>
 * \par
 * The function uses saturating arithmetic.
 * Results outside of the applicable range[0x8000 0x7FFF] will be saturated.
 */
void mrv_offset_qs(FIXP_S *pDst, FIXP_S *pSrc, FIXP_S offset, uint32_t vectorSize);
/** @} */

/**
 * @addtogroup vector_scale
 *
 * Perform element-wise scale operation on a vector.
 *
 * @section code_vector_scale How to use the API
 * - Sample code
 * @include vector_scale.c
 * - Result
 * @include vector_scale_result.txt
 */
/**
 * @addtogroup vector_scale
 * @{
 */
/**
 * @brief Perform element-wise scale operation on a QL format vector.
 * @param[out]      *pDst points to the output vector
 * @param[in]       *pSrc points to the input vector
 * @param[in]       scale scaling factor
 * @param[in]       vectorSize size of the vector
 * @param[in]       q q format specifier, 0 <= q <= 31, Q[32-q].[q]
 * @return none
 *
 * <b>Note</b>
 * \par
 * The function uses saturating arithmetic.
 * Results outside of the applicable range[0x80000000 0x7FFFFFFF] will be saturated.
 */
void mrv_scale_ql(FIXP_L *pDst, FIXP_L *pSrc, FIXP_L scale, uint32_t vectorSize, int16_t q);

/**
 * @brief Perform element-wise scale operation on a QS format vector.
 * @param[out]      *pDst points to the output vector
 * @param[in]       *pSrc points to the input vector
 * @param[in]       scale scaling factor
 * @param[in]       vectorSize size of the vector
 * @param[in]       q q format specifier, 0 <= q <= 15, Q[16-q].[q]
 * @return none
 *
 * <b>Note</b>
 * \par
 * The function uses saturating arithmetic.
 * Results outside of the applicable range[0x8000 0x7FFF] will be saturated.
 */
void mrv_scale_qs(FIXP_S *pDst, FIXP_S *pSrc, FIXP_S scale, uint32_t vectorSize, int16_t q);
/** @} */

/**
 * @addtogroup vector_shift
 *
 * Perform shift operation to the input Q format vector.
 *
 * @section code_vector_shift How to use the API
 * - Sample code
 * @include vector_shift.c
 * - Result
 * @include vector_shift_result.txt
 */
/**
 * @addtogroup vector_shift
 * @{
 */
/**
 * @brief Perform shift operation to the input QL format vector.
 * @param[out]      *pDst points to the output vector
 * @param[in]       *pSrc points to the input vector
 * @param[in]       shiftBits bits to shift, shift left if it is positive, or right if it is negative
 * @param[in]       vectorSize size of the vector
 * @return none
 *
 * <b>Note</b>
 * \par
 * The function uses saturating arithmetic.
 * Results outside of the applicable range[0x80000000 0x7FFFFFFF] will be saturated.
 */
void mrv_shift_ql(FIXP_L *pDst, FIXP_L *pSrc, int32_t shiftBits, uint32_t vectorSize);

/**
 * @brief Perform shift operation to the input QS format vector.
 * @param[out]      *pDst points to the output vector
 * @param[in]       *pSrc points to the input vector
 * @param[in]       shiftBits bits to shift, shift left if it is positive, or right if it is negative
 * @param[in]       vectorSize size of the vector
 * @return none
 *
 * <b>Note</b>
 * \par
 * The function uses saturating arithmetic.
 * Results outside of the applicable range[0x8000 0x7FFF] will be saturated.
 */
void mrv_shift_qs(FIXP_S *pDst, FIXP_S *pSrc, int32_t shiftBits, uint32_t vectorSize);
/** @} */

/**
 * @addtogroup vector_dot
 *
 * Compute the inner product of two Q format vectors.
 *
 * @section code_vector_dot How to use the API
 * - Sample code
 * @include vector_dot.c
 * - Result
 * @include vector_dot_result.txt
 */
/**
 * @addtogroup vector_dot
 * @{
 */
/**
 * Calculate the inner product of two 32-bits Q vectors.
 * @param[in]       *pSrc1 points to the first input vector, in Q 1.31 format
 * @param[in]       *pSrc2 points to the second input vector, in Q 1.31 format
 * @param[in]       vectorSize size of the vector
 * @return inner product value, which is 64-bits in Q16.48 format
 *
 * <b>Input and Output Q Format:</b>
 * \par
 * The input Q format of #mrv_dot_ql() is 1.31\n
 * The output Q format of #mrv_dot_ql() is 16.48
 *
 * <b>Details:</b>
 * \par
 * #mrv_dot_ql() calls #mrv_dot_fast_ql() or #mrv_dot_large_ql() instead of calculating inner products by itself.\n
 * The accumulator of MRV architecture is 72 bits register(GUARD8_ACC64).\n
 * The per element multiplication result, in Q2.62 format(Q1.31 x Q1.31), is added to an accumulator in Q10.62 format.\n
 * When processing large vectors, the partial multiplication result of at most 128 elements, with low order bits truncated\n
 * and interpreted as Q16.48 format, is added to a 64-bits integer type result variable.\n
 * The inner product operation is completed after all vector elements are processed.
 *
 * <b>Overflow Behavior:</b>
 * \par
 * There is no overflow if the vector length is less than or equal to 65536.
 *
 * <b>Getting Better Performance:</b>
 * \par
 * If the length of the vectors is less than or equal to 256 elements, #mrv_dot_ql() calls mrv_dot_fast_ql(), \n
 * which mainly uses ACC register for calculation with less overhead.\n
 * If the length of the vectors is larger than 256 elements, #mrv_dot_ql() calls mrv_dot_large_ql(), \n
 * which uses two-stage accumulation to prevent overflow. Thus, it is slower than #mrv_dot_fast_ql().
 *
 * \par
 * For vectorSize <= 256, it is suggested to call #mrv_dot_fast_ql() directly to get better performance.\n
 * For vectorSize > 256, it is suggested to call #mrv_dot_large_ql() directly to get better performance.\n
 * If not sure the vector size, it is convenient to use #mrv_dot_ql().
 *
 */
FIXP_LL mrv_dot_ql(FIXP_L *pSrc1, FIXP_L *pSrc2, uint32_t vectorSize);

/**
 * Calculate the inner product of two 16-bits Q vectors.
 * @param[in]       *pSrc1 points to the first input vector, in Q 1.15 format
 * @param[in]       *pSrc2 points to the second input vector, in Q 1.15 format
 * @param[in]       vectorSize size of the vector
 * @return inner product value, which is 64-bits in Q32.30 format
 *
 * <b>Input and Output Q Format:</b>
 * \par
 * The input Q format of #mrv_dot_qs() is 1.15\n
 * The output Q format of #mrv_dot_qs() is 32.30
 *
 * <b>Details:</b>
 * \par
 * #mrv_dot_qs() calls #mrv_dot_fast_qs() or #mrv_dot_large_qs() instead of calculating inner products by itself.\n
 * #mrv_dot_fast_qs() leverages the SIMD instruction in MRV ISA.\n
 * One 80 bits ACC register is organized as two 40 bits parts (2 * Guard8_ACC32).\n
 * The SIMD instruction can perform two 16 bits multiplications at a time.\n
 * Each intermediate multiplication result is in Q1.15 x Q1.15 = Q2.30 format.\n
 * The two multiplication results are accumulated to the two 40 bits parts of ACC register respectively.\n
 * After processing all vector elements, sum the two 40 bits parts of ACC register to get the final result.\n
 * If the length of vectors is larger than 256, the two 40 bits parts of ACC register in Q 10.30 may overflow.\n
 * So, #mrv_dot_large_qs() is implemented to support vectors which contain more than 256 elements.\n
 * Obviously, #mrv_dot_large_qs() is slower than #mrv_dot_fast_qs().
 *
 * <b>Overflow Behavior:</b>
 * \par
 * #mrv_dot_qs() supports vector length up to 2^32, and there is no overflow issue.
 *
 * <b>Getting Better Performance:</b>
 * \par
 * If the length of the vectors is less than or equal to 256 elements, #mrv_dot_qs() will call #mrv_dot_fast_qs(), \n
 * which leverages the SIMD instruction in MRV ISA.\n
 * If the length of the vectors is larger than 256 elements, #mrv_dot_qs() will call #mrv_dot_large_qs(), \n
 * which processes the multiplication one by one.
 *
 * \par
 * For vectorSize <= 256, it is suggested to call #mrv_dot_fast_qs() directly to get better performance.\n
 * For vectorSize > 256, it is suggested to call #mrv_dot_large_qs() directly to get better performance.\n
 * If not sure the vector size, it is convenient to use #mrv_dot_qs().
 *
 */
FIXP_LL mrv_dot_qs(FIXP_S *pSrc1, FIXP_S *pSrc2, uint32_t vectorSize);

/**
 * Calculate the inner product of two 32-bits Q vectors with 256 or less elements.
 * The vector size must be less than or equal to 256 to avoid overflow issue.
 *
 * @param[in]       *pSrc1 points to the first input vector, in Q 1.31 format
 * @param[in]       *pSrc2 points to the second input vector, in Q 1.31 format
 * @param[in]       vectorSize size of the vector
 * @return inner product value, which is 64-bits in Q16.48 format
 *
 * <b>Input and Output Q Format:</b>
 * \par
 * The input Q format of #mrv_dot_fast_ql() is 1.31\n
 * The output Q format of #mrv_dot_fast_ql() is 16.48
 *
 * <b>Overflow Behavior:</b>
 * \par
 * There is no overflow if the vector length is less than or equal to 256.
 *
 * <b>Details:</b>
 * \par
 * Please refer to #mrv_dot_ql() for more information.
 *
 */
FIXP_LL mrv_dot_fast_ql(FIXP_L *pSrc1, FIXP_L *pSrc2, uint32_t vectorSize);

/**
 * Calculate the inner product of two 16-bits Q vectors with 256 or less elements.
 * The vector size must be less than or equal to 256 to avoid overflow issue.
 *
 * @param[in]       *pSrc1 points to the first input vector, in Q 1.15 format
 * @param[in]       *pSrc2 points to the second input vector, in Q 1.15 format
 * @param[in]       vectorSize size of the vector
 * @return inner product value, which is 64-bits in Q32.30 format
 *
 * <b>Input and Output Q Format:</b>
 * \par
 * The input Q format of #mrv_dot_fast_qs() is 1.15\n
 * The output Q format of #mrv_dot_fast_qs() is 32.30
 *
 * <b>Overflow Behavior:</b>
 * \par
 * There is no overflow if the vector length is less than or equal to 256.
 *
 * <b>Details:</b>
 * \par
 * #mrv_dot_fast_qs() leverages the SIMD instruction in MRV ISA.\n
 * One 80 bits ACC register is organized as two 40 bits parts (2 * Guard8_ACC32).\n
 * The SIMD instruction can perform two 16 bits multiplications at a time.\n
 * Each intermediate multiplication result is in Q1.15 x Q1.15 = Q2.30 format.\n
 * The two multiplication results are accumulated to the two 40 bits parts of ACC register respectively.\n
 * After processing all vector elements, sum the two 40 bits parts of ACC register to get the final result.
 *
 * \par
 * Please refer to #mrv_dot_qs() for more information.
 *
 */
FIXP_LL mrv_dot_fast_qs(FIXP_S *pSrc1, FIXP_S *pSrc2, uint32_t vectorSize);

/**
 * Calculate the inner product of two 32-bits Q vectors with more than 256 elements.
 * The vector size can be larger than 256. If vector size is less than or equal to 256, use #mrv_dot_fast_ql() will be fast.
 *
 * @param[in]       *pSrc1 points to the first input vector, in Q 1.31 format
 * @param[in]       *pSrc2 points to the second input vector, in Q 1.31 format
 * @param[in]       vectorSize size of the vector
 * @return inner product value, which is 64-bits in Q16.48 format
 *
 * <b>Input and Output Q Format:</b>
 * \par
 * The input Q format of #mrv_dot_large_ql() is 1.31\n
 * The output Q format of #mrv_dot_large_ql() is 16.48
 *
 * <b>Overflow Behavior:</b>
 * \par
 * There is no overflow if the vector length is less than or equal to 65536.
 *
 * <b>Details:</b>
 * \par
 * The accumulator of MRV architecture is 72 bits register(GUARD8_ACC64).\n
 * The per element multiplication result, in Q2.62 format(Q1.31 x Q1.31), is added to an accumulator in Q10.62 format.\n
 * The partial multiplication result of at most 128 elements, with low order bits truncated and interpreted as Q16.48 format,\n
 * is added to a 64-bits integer type result variable. The inner product operation is completed after all vector elements \n
 * are processed.
 * \par
 * Please refer to #mrv_dot_ql() for more information.
 *
 */
FIXP_LL mrv_dot_large_ql(FIXP_L *pSrc1, FIXP_L *pSrc2, uint32_t vectorSize);

/**
 * Calculate the inner product of two 16-bits Q vectors with more than 256 elements.
 * The vector size can be larger than 256. If vector size is less than or equal to 256, use #mrv_dot_fast_qs() will be fast.
 *
 * @param[in]       *pSrc1 points to the first input vector, in Q 1.15 format
 * @param[in]       *pSrc2 points to the second input vector, in Q 1.15 format
 * @param[in]       vectorSize size of the vector
 * @return inner product value, which is 64-bits in Q32.30 format
 *
 * <b>Input and Output Q Format:</b>
 * \par
 * The input Q format of #mrv_dot_large_qs() is 1.15\n
 * The output Q format of #mrv_dot_large_qs() is 32.30
 *
 * <b>Overflow Behavior:</b>
 * \par
 * #mrv_dot_large_qs() supports vector length up to 2^32, and there is no overflow issue.
 *
 * <b>Details:</b>
 * \par
 * Please refer to #mrv_dot_qs() for more information.
 */
FIXP_LL mrv_dot_large_qs(FIXP_S *pSrc1, FIXP_S *pSrc2, uint32_t vectorSize);

/** @} */


/**
 * @addtogroup ldiv
 *
 * Compute the quotient of the 64-bits long integer division.
 *
 * @section code_ldiv How to use the API
 * - Sample code
 * @include ldiv.c
 * - Result
 * @include ldiv_result.txt
 */
/**
 * @addtogroup ldiv
 * @{
 */
/**
 * 64-bits long integer division.
 * @param[out]      pOut points to 64-bits integer for quotient of the division output
 * @param[in]       dividend the dividend of the division in 64-bits integer
 * @param[in]       divisor the divisor of the division in 64-bits integer
 * @return MRV_STATUS_OK if it is OK, or
 *         MRV_STATUS_DIVIDE_BY_ZERO if the divisor is zero.
 */
mrv_status mrv_ldiv(FIXP_LL *pOut, FIXP_LL dividend, FIXP_LL divisor);
/** @} */
/**
 * @addtogroup vector_neg
 *
 * Compute negation of a vector.
 *
 * @section code_vector_neg How to use the API
 * - Sample code
 * @include vector_neg.c
 * - Result
 * @include vector_neg_result.txt
 */
/**
 * @addtogroup vector_neg
 * @{
 */
/**
 * @brief Compute negation of a QL format vector.
 * @param[out]      *pDst points to the output vector
 * @param[in]       *pSrc points to the input vector
 * @param[in]       vectorSize size of the vector
 * @return none
 *
 * <b>Note</b>
 * \par
 * The function uses saturating arithmetic.
 * Results outside of the applicable range[0x80000000 0x7FFFFFFF] will be saturated.
 */
void mrv_neg_ql(FIXP_L *pDst, FIXP_L *pSrc, uint32_t vectorSize);

/**
 * @brief Compute negation of a QS format vector.
 * @param[out]      *pDst points to the output vector
 * @param[in]       *pSrc points to the input vector
 * @param[in]       vectorSize size of the vector
 * @return none
 *
 * <b>Note</b>
 * \par
 * The function uses saturating arithmetic.
 * Results outside of the applicable range[0x8000 0x7FFF] will be saturated.
 */
void mrv_neg_qs(FIXP_S *pDst, FIXP_S *pSrc, uint32_t vectorSize);
/** @} */

/**
 * @addtogroup vector_abs
 *
 * Compute element-wise absolute value of a vector.
 *
 * @section code_vector_abs How to use the API
 * - Sample code
 * @include vector_abs.c
 * - Result
 * @include vector_abs_result.txt
 */
/**
 * @addtogroup vector_abs
 * @{
 */
/**
 * @brief Compute element-wise absolute value of a QL format vector.
 * @param[out]      *pDst points to the output vector
 * @param[in]       *pSrc points to the input vector
 * @param[in]       vectorSize size of the vector
 * @return none
 *
 * <b>Note</b>
 * \par
 * The function uses saturating arithmetic.
 * Results outside of the applicable range[0x80000000 0x7FFFFFFF] will be saturated.
 */
void mrv_abs_ql(FIXP_L *pDst, FIXP_L *pSrc, uint32_t vectorSize);

/**
 * @brief Compute element-wise absolute value of a QS format vector.
 * @param[out]      *pDst points to the output vector
 * @param[in]       *pSrc points to the input vector
 * @param[in]       vectorSize size of the vector
 * @return none
 *
 * <b>Note</b>
 * \par
 * The function uses saturating arithmetic.
 * Results outside of the applicable range[0x8000 0x7FFF] will be saturated.
 */
void mrv_abs_qs(FIXP_S *pDst, FIXP_S *pSrc, uint32_t vectorSize);
/** @} */


/**
 * @addtogroup vector_mul
 *
 * Compute multiplication of two vectors.
 *
 * @section code_vector_mul How to use the API
 * - Sample code
 * @include vector_mul.c
 * - Result
 * @include vector_mul_result.txt
 */
/**
 * @addtogroup vector_mul
 * @{
 */
/**
 * @brief Compute multiplication of two QL format vectors.
 * @param[out]      *pDst points to the output vector
 * @param[in]       *pSrc1 points to the first vector
 * @param[in]       *pSrc2 points to the second vector
 * @param[in]       vectorSize size of the vector
 * @param[in]       q q format specifier, 0 <= q <= 31, Q[32-q].[q]
 * @return none
 *
 * <b>Note</b>
 * \par
 * The function uses saturating arithmetic.
 * Results outside of the applicable range[0x80000000 0x7FFFFFFF] will be saturated.
 */
void mrv_mul_ql(FIXP_L *pDst, FIXP_L *pSrc1, FIXP_L *pSrc2, uint32_t vectorSize, int16_t q);

/**
 * @brief Compute multiplication of two QS format vectors.
 * @param[out]      *pDst points to the output vector
 * @param[in]       *pSrc1 points to the first vector
 * @param[in]       *pSrc2 points to the second vector
 * @param[in]       vectorSize size of the vector
 * @param[in] q q format specifier, 0 <= q <= 15, Q[16-q].[q]
 * @return none
 *
 * <b>Note</b>
 * \par
 * The function uses saturating arithmetic.
 * Results outside of the applicable range[0x8000 0x7FFF] will be saturated.
 */
void mrv_mul_qs(FIXP_S *pDst, FIXP_S *pSrc1, FIXP_S *pSrc2, uint32_t vectorSize, int16_t q);
/** @} */

#endif
