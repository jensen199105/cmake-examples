#ifndef __KISS_FFT_H__
#define __KISS_FFT_H__

#ifdef __KERNEL__
#include <linux/kernel.h>
#endif

/*
 ATTENTION!
 If you would like a :
 -- a utility that will handle the caching of fft objects
 -- real-only (no imaginary time component ) FFT
 -- a multi-dimensional FFT
 -- a command-line utility to perform ffts
 -- a command-line utility to perform fast-convolution filtering

 Then see kfc.h kiss_fftr.h kiss_fftnd.h fftutil.c kiss_fastfir.c
  in the tools/ directory.
*/

#define kiss_fft_scalar S32

typedef struct
{
    kiss_fft_scalar r;
    kiss_fft_scalar i;
} kiss_fft_cpx;

#define MAXFACTORS 32
/* e.g. an fft of length 128 has 4 factors
 as far as kissfft is concerned
 4*4*4*2
 */
typedef struct kiss_fft_state *kiss_fft_cfg;
struct kiss_fft_state
{
    S32 nfft;
    S32 inverse;
    S32 factors[2 * MAXFACTORS];
    kiss_fft_cpx *twiddles;
};

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  kiss_fft_alloc
 *
 *  Initialize a FFT (or IFFT) algorithm's cfg/state buffer.
 *
 *  typical usage:      kiss_fft_cfg mycfg=kiss_fft_alloc(1024,0,NULL,NULL);
 *
 *  The return value from fft_alloc is a cfg buffer used internally
 *  by the fft routine or NULL.
 *
 *  If lenmem is NULL, then kiss_fft_alloc will allocate a cfg buffer using malloc.
 *  The returned value should be free()d when done to avoid memory leaks.
 *
 *  The state can be placed in a user supplied buffer 'mem':
 *  If lenmem is not NULL and mem is not NULL and *lenmem is large enough,
 *      then the function places the cfg in mem and the size used in *lenmem
 *      and returns mem.
 *
 *  If lenmem is not NULL and ( mem is NULL or *lenmem is not large enough),
 *      then the function returns NULL and places the minimum cfg
 *      buffer size in *lenmem.
 * */

kiss_fft_cfg kiss_fft_alloc(S32 nfft, S32 inverse_fft, VOID *mem, S32 *lenmem);

/*
 * kiss_fft(cfg,in_out_buf)
 *
 * Perform an FFT on a complex input buffer.
 * for a forward FFT,
 * fin should be  f[0] , f[1] , ... ,f[nfft-1]
 * fout will be   F[0] , F[1] , ... ,F[nfft-1]
 * Note that each element is complex and can be accessed like
    f[k].r and f[k].i
 * */
VOID kiss_fft(kiss_fft_cfg cfg, CONST kiss_fft_cpx *fin, kiss_fft_cpx *fout);

/*
 A more generic version of the above function. It reads its input from every Nth sample.
 * */
VOID kiss_fft_stride(kiss_fft_cfg cfg, CONST kiss_fft_cpx *fin, kiss_fft_cpx *fout, S32 fin_stride);

/*
 Cleans up some memory that gets managed internally. Not necessary to call, but it might clean up
 your compiler output to call this before you exit.
*/
VOID kiss_fft_cleanup(VOID);


/*
 * Returns the smallest integer k, such that k>=n and k has only "fast" factors (2,3,5)
 */
S32 kiss_fft_next_fast_size(S32 n);

#ifdef __cplusplus
}
#endif

#endif
