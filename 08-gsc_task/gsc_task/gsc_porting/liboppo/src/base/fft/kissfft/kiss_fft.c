/*
Copyright (c) 2003-2010, Mark Borgerding

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the author nor the names of any contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "AISP_TSL_base.h"
#include "_kiss_fft_guts.h"

/* The guts header contains all the multiplication and addition macros that are defined for
 fixed or floating point complex numbers.  It also delares the kf_ internal functions.
 */

STATIC VOID kf_bfly2(kiss_fft_cpx *Fout, CONST S32 fstride, kiss_fft_cfg st, S32 m)
{
    kiss_fft_cpx *Fout2 = NULL;
    kiss_fft_cpx *tw1 = st->twiddles;
    kiss_fft_cpx t;
    Fout2 = Fout + m;

    do
    {
        C_SHR1_ROUND(*Fout);
        C_SHR1_ROUND(*Fout2);
        C_MUL(t,  *Fout2, *tw1);
        tw1 += fstride;
        C_SUB(*Fout2,  *Fout, t);
        C_ADDTO(*Fout,  t);
        ++Fout2;
        ++Fout;
    }
    while (--m);
}

STATIC VOID kf_bfly4(kiss_fft_cpx *Fout, CONST S32 fstride, kiss_fft_cfg st, CONST S32 m)
{
    kiss_fft_cpx *tw1 = NULL;
    kiss_fft_cpx *tw2 = NULL;
    kiss_fft_cpx *tw3 = NULL;
    kiss_fft_cpx scratch[6];
    S32 k        = m;
    CONST S32 m2 = (m << 1);
    CONST S32 m3 = (m + m2);

    tw3 = tw2 = tw1 = st->twiddles;

    do
    {
        C_SHR2_ROUND(*Fout);
        C_SHR2_ROUND(Fout[m]);
        C_SHR2_ROUND(Fout[m2]);
        C_SHR2_ROUND(Fout[m3]);
        C_MUL(scratch[0], Fout[m], *tw1);
        C_MUL(scratch[1], Fout[m2], *tw2);
        C_MUL(scratch[2], Fout[m3], *tw3);
        C_SUB(scratch[5], *Fout, scratch[1]);
        C_ADDTO(*Fout, scratch[1]);
        C_ADD(scratch[3], scratch[0], scratch[2]);
        C_SUB(scratch[4], scratch[0], scratch[2]);
        C_SUB(Fout[m2], *Fout, scratch[3]);
        tw1 += fstride;
        tw2 += (fstride << 1);
        tw3 += fstride + (fstride << 1);
        C_ADDTO(*Fout, scratch[3]);

        if (st->inverse)
        {
            Fout[m].r  = scratch[5].r - scratch[4].i;
            Fout[m].i  = scratch[5].i + scratch[4].r;
            Fout[m3].r = scratch[5].r + scratch[4].i;
            Fout[m3].i = scratch[5].i - scratch[4].r;
        }
        else
        {
            Fout[m].r  = scratch[5].r + scratch[4].i;
            Fout[m].i  = scratch[5].i - scratch[4].r;
            Fout[m3].r = scratch[5].r - scratch[4].i;
            Fout[m3].i = scratch[5].i + scratch[4].r;
        }

        ++Fout;
    }
    while (--k);
}

STATIC VOID kf_bfly3(kiss_fft_cpx *Fout, CONST S32 fstride, kiss_fft_cfg st, S32 m)
{
    S32 k = m;
    CONST S32 m2 = 2 * m;
    kiss_fft_cpx *tw1 = NULL;
    kiss_fft_cpx *tw2 = NULL;
    kiss_fft_cpx scratch[5];
    kiss_fft_cpx epi3;

    epi3 = st->twiddles[fstride * m];
    tw1 = tw2 = st->twiddles;

    do
    {
        C_FIXDIV(*Fout, 3);
        C_FIXDIV(Fout[m], 3);
        C_FIXDIV(Fout[m2], 3);
        C_MUL(scratch[1], Fout[m], *tw1);
        C_MUL(scratch[2], Fout[m2], *tw2);
        C_ADD(scratch[3], scratch[1], scratch[2]);
        C_SUB(scratch[0], scratch[1], scratch[2]);
        tw1 += fstride;
        tw2 += fstride * 2;
        Fout[m].r = Fout->r - HALF_OF(scratch[3].r);
        Fout[m].i = Fout->i - HALF_OF(scratch[3].i);
        C_MULBYSCALAR(scratch[0], epi3.i);
        C_ADDTO(*Fout, scratch[3]);
        Fout[m2].r = Fout[m].r + scratch[0].i;
        Fout[m2].i = Fout[m].i - scratch[0].r;
        Fout[m].r -= scratch[0].i;
        Fout[m].i += scratch[0].r;
        ++Fout;
    }
    while (--k);
}

STATIC VOID kf_bfly5(kiss_fft_cpx *Fout, CONST S32 fstride, kiss_fft_cfg st, S32 m)
{
    kiss_fft_cpx *Fout0 = NULL;
    kiss_fft_cpx *Fout1 = NULL;
    kiss_fft_cpx *Fout2 = NULL;
    kiss_fft_cpx *Fout3 = NULL;
    kiss_fft_cpx *Fout4 = NULL;
    S32 u = fstride * m;
    kiss_fft_cpx scratch[13];
    kiss_fft_cpx *twiddles = st->twiddles;
    kiss_fft_cpx *tw = NULL;
    kiss_fft_cpx ya;
    kiss_fft_cpx yb;

    ya = twiddles[u];
    yb = twiddles[u << 1];
    Fout0 = Fout;
    Fout1 = Fout0 + m;
    Fout2 = Fout0 + (m << 1);
    Fout3 = Fout0 + (m << 2) - m;
    Fout4 = Fout0 + (m << 2);
    tw = st->twiddles;

    for (u = 0; u < m; ++u)
    {
        C_FIXDIV(*Fout0, 5);
        C_FIXDIV(*Fout1, 5);
        C_FIXDIV(*Fout2, 5);
        C_FIXDIV(*Fout3, 5);
        C_FIXDIV(*Fout4, 5);
        scratch[0] = *Fout0;
        C_MUL(scratch[1], *Fout1, tw[u * fstride]);
        C_MUL(scratch[2], *Fout2, tw[2 * u * fstride]);
        C_MUL(scratch[3], *Fout3, tw[3 * u * fstride]);
        C_MUL(scratch[4], *Fout4, tw[4 * u * fstride]);
        C_ADD(scratch[7], scratch[1], scratch[4]);
        C_SUB(scratch[10], scratch[1], scratch[4]);
        C_ADD(scratch[8], scratch[2], scratch[3]);
        C_SUB(scratch[9], scratch[2], scratch[3]);
        Fout0->r += scratch[7].r + scratch[8].r;
        Fout0->i += scratch[7].i + scratch[8].i;
        scratch[5].r = scratch[0].r + S_MUL(scratch[7].r, ya.r) + S_MUL(scratch[8].r, yb.r);
        scratch[5].i = scratch[0].i + S_MUL(scratch[7].i, ya.r) + S_MUL(scratch[8].i, yb.r);
        scratch[6].r =  S_MUL(scratch[10].i, ya.i) + S_MUL(scratch[9].i, yb.i);
        scratch[6].i = -S_MUL(scratch[10].r, ya.i) - S_MUL(scratch[9].r, yb.i);
        C_SUB(*Fout1, scratch[5], scratch[6]);
        C_ADD(*Fout4, scratch[5], scratch[6]);
        scratch[11].r = scratch[0].r + S_MUL(scratch[7].r, yb.r) + S_MUL(scratch[8].r, ya.r);
        scratch[11].i = scratch[0].i + S_MUL(scratch[7].i, yb.r) + S_MUL(scratch[8].i, ya.r);
        scratch[12].r = - S_MUL(scratch[10].i, yb.i) + S_MUL(scratch[9].i, ya.i);
        scratch[12].i = S_MUL(scratch[10].r, yb.i) - S_MUL(scratch[9].r, ya.i);
        C_ADD(*Fout2, scratch[11], scratch[12]);
        C_SUB(*Fout3, scratch[11], scratch[12]);
        ++Fout0;
        ++Fout1;
        ++Fout2;
        ++Fout3;
        ++Fout4;
    }
}

STATIC VOID kf_work(kiss_fft_cpx *Fout, CONST kiss_fft_cpx *f, CONST S32 fstride, S32 in_stride, S32 *factors,
                    kiss_fft_cfg st)
{
    kiss_fft_cpx *Fout_beg = Fout;
    CONST S32 p = *factors++; /* the radix  */
    CONST S32 m = *factors++; /* stage's fft length/p */
    CONST S32 iScalar = fstride * in_stride;
    CONST kiss_fft_cpx *Fout_end = Fout + p * m;

    if (m == 1)
    {
        do
        {
            *Fout = *f;
            f += iScalar;
        }
        while (++Fout != Fout_end);
    }
    else
    {
        do
        {
            // recursive call:
            // DFT of size m*p performed by doing
            // p instances of smaller DFTs of size m,
            // each one takes a decimated version of the input
            kf_work(Fout, f, fstride * p, in_stride, factors, st);
            f += iScalar;
        }
        while ((Fout += m) != Fout_end);
    }

    Fout = Fout_beg;

    // recombine the p smaller DFTs
    switch (p)
    {
    case 2:
        kf_bfly2(Fout, fstride, st, m);
        break;

    case 3:
        kf_bfly3(Fout, fstride, st, m);
        break;

    case 4:
        kf_bfly4(Fout, fstride, st, m);
        break;

    case 5:
        kf_bfly5(Fout, fstride, st, m);
        break;

    default:
        //TODO: handle error here
        break;
    }
}

VOID kiss_fft_stride(kiss_fft_cfg cfg, CONST kiss_fft_cpx *fin, kiss_fft_cpx *fout, S32 in_stride)
{
    kf_work(fout, fin, 1, in_stride, cfg->factors, cfg);
}

VOID kiss_fft(kiss_fft_cfg cfg, CONST kiss_fft_cpx *fin, kiss_fft_cpx *fout)
{
    kiss_fft_stride(cfg, fin, fout, 1);
}

VOID kiss_fft_cleanup(VOID)
{
    /* nothing needed any more */
}

S32 kiss_fft_next_fast_size(S32 n)
{
    while (1)
    {
        S32 m = n;

        while ((m & 1) == 0)
        {
            m >>= 1;
        }

        while ((m & 3) == 0)
        {
            m /= 3;
        }

        while ((m % 5) == 0)
        {
            m /= 5;
        }

        if (m <= 1)
        {
            break;    /* n is completely factorable by twos, threes, and fives */
        }

        n++;
    }

    return n;
}
