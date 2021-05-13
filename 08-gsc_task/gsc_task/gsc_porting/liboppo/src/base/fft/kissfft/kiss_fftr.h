#ifndef __KISS_FFTR_H__
#define __KISS_FFTR_H__

#include "_kiss_fft_guts.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct kiss_fftr_state *kiss_fftr_cfg;
struct kiss_fftr_state
{
    kiss_fft_cfg substate;
    kiss_fft_cpx *tmpbuf;
    kiss_fft_cpx *super_twiddles;
};

kiss_fftr_cfg kiss_fftr_alloc(S32 nfft, S32 inverse_fft, VOID *mem, S32 *lenmem);
/*
 nfft must be even

 If you don't care to allocate space, use mem = lenmem = NULL
*/


VOID kiss_fftr(kiss_fftr_cfg cfg, CONST kiss_fft_scalar *timedata, kiss_fft_cpx *freqdata);
/*
 input timedata has nfft scalar points
 output freqdata has nfft/2+1 complex points
*/

VOID kiss_fftri(kiss_fftr_cfg cfg, CONST kiss_fft_cpx *freqdata, kiss_fft_scalar *timedata);
/*
 input freqdata has  nfft/2+1 complex points
 output timedata has nfft scalar points
*/

#ifdef __cplusplus
}
#endif
#endif
