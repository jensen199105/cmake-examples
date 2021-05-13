#ifndef __AISP_TSL_WIN_H__
#define __AISP_TSL_WIN_H__

#ifndef __AISP_TSL_BASE_H__
#error "forbid to include this header directly"
#endif

typedef enum
{
    WIN_HANNING_512,
    WIN_HANNING_SQRT_256,
    WIN_HANNING_SQRT_512,
    WIN_HANNING_SQRT_1024,
} WINDOW_EN;

CONST VOID *AISP_TSL_get_win(WINDOW_EN enType);

#endif
