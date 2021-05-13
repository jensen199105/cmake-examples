#include "AISP_TSL_base.h"
#include "gsc_core_tables.h"

/*
 * THe tables list in this file is including:
 *     1. g_aHanning_15
 */

/* Hanning_15 window, length is 31 */
CONST GSC_S32_T g_aHanning_15Window31[31] =
{
#ifdef GSC_FIXED
    /* Q31 */
    1289478,         5108358,        11309883,        19655731,
    29825177,        41427414,        54016574,        67108864,
    80201154,        92790314,       104392551,       114561997,
    122907845,       129109370,       132928250,       134217728,
    132928250,       129109370,       122907845,       114561997,
    104392551,        92790314,        80201154,        67108864,
    54016574,        41427414,        29825177,        19655731,
    11309883,         5108358,         1289478,
#else
    0.000600459987399, 0.002378764609022, 0.005266574615545, 0.009152913087920,
    0.013888430218137, 0.019291142738591, 0.025153427436996, 0.031250000000000,
    0.037346572563004, 0.043208857261409, 0.048611569781863, 0.053347086912080,
    0.057233425384455, 0.060121235390978, 0.061899540012601, 0.062500000000000,
    0.061899540012601, 0.060121235390978, 0.057233425384455, 0.053347086912080,
    0.048611569781863, 0.043208857261409, 0.037346572563004, 0.031250000000000,
    0.025153427436996, 0.019291142738591, 0.013888430218137, 0.009152913087920,
    0.005266574615545, 0.002378764609022, 0.000600459987399,
#endif
};
