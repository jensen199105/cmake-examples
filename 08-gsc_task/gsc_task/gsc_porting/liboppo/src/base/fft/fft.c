#include "AISP_TSL_base.h"
#include "kissfft/kiss_fftr.h"

struct AISP_FFT_ENGINE
{
    BOOL inverse;
    S32 npoint;
    S32 inQ;
    S32 outQ;
    kiss_fftr_cfg cfg;
};

#ifdef FFT_512
extern CONST S32 fft_table_512[];
extern CONST S32 fft_supertable_512[];
extern CONST S32 ifft_table_512[];
extern CONST S32 ifft_supertable_512[];
ATTR_ALING(8) STATIC BYTE aisp_fftHeap_512[2368];
ATTR_ALING(8) STATIC BYTE aisp_ifftHeap_512[2368];
#endif

#ifdef FFT_1024
extern CONST S32 fft_table_1024[];
extern CONST S32 fft_supertable_1024[];
extern CONST S32 ifft_table_1024[];
extern CONST S32 ifft_supertable_1024[];
ATTR_ALING(8) STATIC BYTE aisp_fftHeap_1024[4416];
ATTR_ALING(8) STATIC BYTE aisp_ifftHeap_1024[4416];
#endif

/*  facbuf is populated by p1,m1,p2,m2, ...
 *  where
 *  p[i] * m[i] = m[i-1]
 *  m0 = n
 */
STATIC VOID kf_factor_fixed(S32 n, S32 *facbuf)
{
    S32 p = 4;
    //TODO: support other fft points
    S32 floor_sqrt = 22; /* n = 512 by default */

    /*factor out powers of 4, powers of 2, then any remaining primes */
    do
    {
        while (n % p)
        {
            switch (p)
            {
            case 4:
                p = 2;
                break;

            case 2:
                p = 3;
                break;

            default:
                p += 2;
                break;
            }

            if (p > floor_sqrt)
            {
                p = n;    /* no more factors, skip to end */
            }
        }

        n /= p;
        *facbuf++ = p;
        *facbuf++ = n;
    }
    while (n > 1);
}

AISP_FFT_ENGINE_S *AISP_fft_new(BOOL inverse, S32 npoint, S32 inQ, S32 outQ)
{
    AISP_FFT_ENGINE_S *engine = NULL;
    BYTE *heap = NULL;
    S32 nfft = npoint / 2;
    kiss_fft_cpx *table = NULL;
    kiss_fft_cpx *supertable = NULL;
    kiss_fftr_cfg cfg = NULL;
    kiss_fft_cfg substate = NULL;

    // NOLINTNEXTLINE(hicpp-multiway-paths-covered)
    switch (npoint)
    {
#ifdef FFT_512

    case 512:
        heap = !inverse ? aisp_fftHeap_512 : aisp_ifftHeap_512;
        table = (kiss_fft_cpx *)(!inverse ? fft_table_512 : ifft_table_512);
        supertable = (kiss_fft_cpx *)(!inverse ? fft_supertable_512 : ifft_supertable_512);
        break;
#endif
#ifdef FFT_1024

    case 1024:
        heap = !inverse ? aisp_fftHeap_1024 : aisp_ifftHeap_1024;
        table = (kiss_fft_cpx *)(!inverse ? fft_table_1024 : ifft_table_1024);
        supertable = (kiss_fft_cpx *)(!inverse ? fft_supertable_1024 : ifft_supertable_1024);
        break;
#endif

    default:
        AISP_TSL_printf("unsupported FFT points: %d\n", npoint);
        return NULL;
    }

    engine = (AISP_FFT_ENGINE_S *)heap;
    heap += sizeof(AISP_FFT_ENGINE_S);
    cfg = (kiss_fftr_cfg)heap;
    heap += sizeof(struct kiss_fftr_state);
    substate = (kiss_fft_cfg)heap;
    heap += sizeof(struct kiss_fft_state);
    cfg->tmpbuf = (kiss_fft_cpx *)heap;
    //heap += sizeof(kiss_fft_cpx) * nfft;
    kf_factor_fixed(nfft, &substate->factors[0]);
    substate->nfft = nfft;
    substate->inverse = inverse;
    substate->twiddles = table;
    cfg->super_twiddles = supertable;
    cfg->substate = substate;
    engine->inverse = inverse;
    engine->npoint = npoint;
    engine->inQ = inQ;
    engine->outQ = outQ;
    engine->cfg = cfg;
    return engine;
}

VOID AISP_fft_delete(AISP_FFT_ENGINE_S *engine)
{
}

S32 AISP_fft_feed(AISP_FFT_ENGINE_S *engine, CONST VOID *in, VOID *out)
{
    if (!engine->inverse)
    {
        kiss_fftr(engine->cfg, in, out);
    }
    else
    {
        kiss_fftri(engine->cfg, in, out);
    }

    //TODO: use inQ & outQ to do out data shapping
    return 0;
}
