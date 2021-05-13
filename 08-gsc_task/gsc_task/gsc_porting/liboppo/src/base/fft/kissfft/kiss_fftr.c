/*
Copyright (c) 2003-2004, Mark Borgerding

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the author nor the names of any contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "AISP_TSL_base.h"
#include "kiss_fftr.h"
#include "_kiss_fft_guts.h"

VOID kiss_fftr(kiss_fftr_cfg cfg, CONST kiss_fft_scalar *timedata, kiss_fft_cpx *freqdata)
{
    /* input buffer timedata is stored row-wise */
    S32 k = 0;
    S32 ncfft = 0;
    kiss_fft_cpx fpnk;
    kiss_fft_cpx fpk;
    kiss_fft_cpx f1k;
    kiss_fft_cpx f2k;
    kiss_fft_cpx tw;
    kiss_fft_cpx tdc;

    if (cfg->substate->inverse)
    {
        AISP_TSL_printf("kiss fft usage error: improper alloc\n");
        return;
    }

    ncfft = cfg->substate->nfft;
    /*perform the parallel fft of two real signals packed in real,imag*/
    kiss_fft(cfg->substate, (CONST kiss_fft_cpx *)timedata, cfg->tmpbuf);
    /* The real part of the DC element of the frequency spectrum in st->tmpbuf
     * contains the sum of the even-numbered elements of the input time sequence
     * The imag part is the sum of the odd-numbered elements
     *
     * The sum of tdc.r and tdc.i is the sum of the input time sequence.
     *      yielding DC of input time sequence
     * The difference of tdc.r - tdc.i is the sum of the input (dot product) [1,-1,1,-1...
     *      yielding Nyquist bin of input time sequence
     */
    tdc.r = cfg->tmpbuf[0].r;
    tdc.i = cfg->tmpbuf[0].i;
    C_SHR1_ROUND(tdc);
    CHECK_OVERFLOW_OP(tdc.r, +, tdc.i);
    CHECK_OVERFLOW_OP(tdc.r, -, tdc.i);
    freqdata[0].r = tdc.r + tdc.i;
    freqdata[ncfft].r = tdc.r - tdc.i;
    freqdata[ncfft].i = freqdata[0].i = 0;

    for (k = 1; k <= ncfft / 2 ; ++k)
    {
        fpk    = cfg->tmpbuf[k];
        fpnk.r =   cfg->tmpbuf[ncfft - k].r;
        fpnk.i = - cfg->tmpbuf[ncfft - k].i;
        C_SHR1_ROUND(fpk);
        C_SHR1_ROUND(fpnk);
        C_ADD(f1k, fpk, fpnk);
        C_SUB(f2k, fpk, fpnk);
        C_MUL(tw, f2k, cfg->super_twiddles[k - 1]);
        freqdata[k].r = HALF_OF(f1k.r + tw.r);
        freqdata[k].i = HALF_OF(f1k.i + tw.i);
        freqdata[ncfft - k].r = HALF_OF(f1k.r - tw.r);
        freqdata[ncfft - k].i = HALF_OF(tw.i - f1k.i);
    }
}

VOID kiss_fftri(kiss_fftr_cfg cfg, CONST kiss_fft_cpx *freqdata, kiss_fft_scalar *timedata)
{
    /* input buffer timedata is stored row-wise */
    S32 k = 0;
    S32 ncfft = 0;
    S32 iLoopCnt = 0;

    if (cfg->substate->inverse == 0)
    {
        AISP_TSL_printf("kiss fft usage error: improper alloc\n");
        return ;
    }

    ncfft = cfg->substate->nfft;
    iLoopCnt = ncfft >> 1;
    cfg->tmpbuf[0].r = freqdata[0].r + freqdata[ncfft].r;
    cfg->tmpbuf[0].i = freqdata[0].r - freqdata[ncfft].r;
    C_SHR1_ROUND(cfg->tmpbuf[0]);

    for (k = 1; k <= iLoopCnt; ++k)
    {
        kiss_fft_cpx fk;
        kiss_fft_cpx fnkc;
        kiss_fft_cpx fek;
        kiss_fft_cpx fok;
        kiss_fft_cpx tmp;

        fk = freqdata[k];
        fnkc.r = freqdata[ncfft - k].r;
        fnkc.i = -freqdata[ncfft - k].i;
        C_SHR1_ROUND(fk);
        C_SHR1_ROUND(fnkc);
        C_ADD(fek, fk, fnkc);
        C_SUB(tmp, fk, fnkc);
        C_MUL(fok, tmp, cfg->super_twiddles[k - 1]);
        C_ADD(cfg->tmpbuf[k],     fek, fok);
        C_SUB(cfg->tmpbuf[ncfft - k], fek, fok);
        cfg->tmpbuf[ncfft - k].i *= -1;
    }

    kiss_fft(cfg->substate, cfg->tmpbuf, (kiss_fft_cpx *) timedata);
}
