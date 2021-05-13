#include "AISP_TSL_base.h"

#ifdef HANNING_512
extern CONST S32 hanning_win_512[];
#endif
#ifdef HANNING_SQRT_256
extern CONST S32 hanning_win_sqrt_256[];
#endif
#ifdef HANNING_SQRT_512
extern CONST S32 hanning_win_sqrt_512[];
#endif
#ifdef HANNING_SQRT_1024
extern CONST S32 hanning_win_sqrt_1024[];
#endif

CONST VOID *AISP_TSL_get_win(WINDOW_EN enType)
{
    // NOLINTNEXTLINE(hicpp-multiway-paths-covered)
    switch (enType)
    {
#ifdef HANNING_512

    case WIN_HANNING_512:
        return hanning_win_512;
#endif
#ifdef HANNING_SQRT_256

    case WIN_HANNING_SQRT_256:
        return hanning_win_sqrt_256;
#endif
#ifdef HANNING_SQRT_512

    case WIN_HANNING_SQRT_512:
        return hanning_win_sqrt_512;
#endif
#ifdef HANNING_SQRT_1024

    case WIN_HANNING_SQRT_1024:
        return hanning_win_sqrt_1024;
#endif

    default:
        AISP_TSL_printf("unsupported window type: %d\n", enType);
        return NULL;
    }
}
