#ifndef __AISP_TSL_FFT_H__
#define __AISP_TSL_FFT_H__

#ifndef __AISP_TSL_BASE_H__
#error "forbid to include this header directly"
#endif

typedef struct AISP_FFT_ENGINE AISP_FFT_ENGINE_S;

AISP_FFT_ENGINE_S *AISP_fft_new(BOOL inverse, S32 npoint, S32 inQ, S32 outQ);
VOID AISP_fft_delete(AISP_FFT_ENGINE_S *engine);
S32 AISP_fft_feed(AISP_FFT_ENGINE_S *engine, CONST VOID *in, VOID *out);

#endif
