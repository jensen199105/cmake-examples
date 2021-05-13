#include "AISP_TSL_base.h"

#ifdef HANNING_SQRT_512
/* Hanning 512 window is W32Q31 */
CONST S32 hanning_win_sqrt_512[] =
{
    13151027,        26301560,        39451107,        52599175,        65745270,        78888899,        92029570,       105166789,
    118300065,       131428903,       144552813,       157671302,       170783877,       183890048,       196989322,       210081209,
    223165217,       236240856,       249307635,       262365064,       275412654,       288449915,       301476358,       314491496,
    327494838,       340485899,       353464191,       366429227,       379380521,       392317587,       405239940,       418147095,
    431038568,       443913877,       456772537,       469614067,       482437986,       495243811,       508031064,       520799264,
    533547932,       546276591,       558984763,       571671972,       584337741,       596981596,       609603063,       622201668,
    634776938,       647328402,       659855590,       672358032,       684835258,       697286801,       709712193,       722110970,
    734482665,       746826815,       759142957,       771430629,       783689371,       795918721,       808118223,       820287418,
    832425849,       844533063,       856608604,       868652020,       880662859,       892640671,       904585006,       916495417,
    928371456,       940212679,       952018642,       963788901,       975523015,       987220544,       998881050,      1010504095,
    1022089244,      1033636061,      1045144114,      1056612971,      1068042202,      1079431379,      1090780074,      1102087861,
    1113354317,      1124579020,      1135761547,      1146901480,      1157998402,      1169051895,      1180061545,      1191026940,
    1201947667,      1212823319,      1223653486,      1234437763,      1245175745,      1255867029,      1266511215,      1277107903,
    1287656696,      1298157198,      1308609016,      1319011757,      1329365031,      1339668451,      1349921629,      1360124182,
    1370275726,      1380375881,      1390424268,      1400420510,      1410364232,      1420255062,      1430092629,      1439876562,
    1449606497,      1459282067,      1468902910,      1478468665,      1487978973,      1497433478,      1506831825,      1516173662,
    1525458638,      1534686404,      1543856616,      1552968929,      1562023001,      1571018493,      1579955068,      1588832389,
    1597650125,      1606407945,      1615105520,      1623742524,      1632318633,      1640833525,      1649286882,      1657678385,
    1666007722,      1674274578,      1682478644,      1690619613,      1698697178,      1706711038,      1714660891,      1722546440,
    1730367389,      1738123443,      1745814314,      1753439711,      1760999350,      1768492946,      1775920218,      1783280889,
    1790574682,      1797801323,      1804960541,      1812052069,      1819075639,      1826030989,      1832917858,      1839735987,
    1846485121,      1853165007,      1859775393,      1866316033,      1872786681,      1879187094,      1885517033,      1891776259,
    1897964538,      1904081639,      1910127330,      1916101387,      1922003585,      1927833702,      1933591520,      1939276823,
    1944889398,      1950429033,      1955895523,      1961288660,      1966608244,      1971854075,      1977025955,      1982123692,
    1987147093,      1992095971,      1996970140,      2001769416,      2006493621,      2011142577,      2015716109,      2020214046,
    2024636219,      2028982463,      2033252615,      2037446513,      2041564002,      2045604927,      2049569135,      2053456479,
    2057266813,      2060999993,      2064655880,      2068234337,      2071735229,      2075158425,      2078503797,      2081771219,
    2084960570,      2088071728,      2091104577,      2094059005,      2096934899,      2099732153,      2102450660,      2105090320,
    2107651033,      2110132703,      2112535238,      2114858546,      2117102541,      2119267139,      2121352259,      2123357822,
    2125283753,      2127129980,      2128896433,      2130583047,      2132189759,      2133716507,      2135163235,      2136529888,
    2137816415,      2139022769,      2140148903,      2141194775,      2142160347,      2143045581,      2143850446,      2144574910,
    2145218946,      2145782531,      2146265643,      2146668264,      2146990378,      2147231975,      2147393045,      2147473581,
    2147473581,      2147393045,      2147231975,      2146990378,      2146668264,      2146265643,      2145782531,      2145218946,
    2144574910,      2143850446,      2143045581,      2142160347,      2141194775,      2140148903,      2139022769,      2137816415,
    2136529888,      2135163235,      2133716507,      2132189759,      2130583047,      2128896433,      2127129980,      2125283753,
    2123357822,      2121352259,      2119267139,      2117102541,      2114858546,      2112535238,      2110132703,      2107651033,
    2105090320,      2102450660,      2099732153,      2096934899,      2094059005,      2091104577,      2088071728,      2084960570,
    2081771219,      2078503797,      2075158425,      2071735229,      2068234337,      2064655880,      2060999993,      2057266813,
    2053456479,      2049569135,      2045604927,      2041564002,      2037446513,      2033252615,      2028982463,      2024636219,
    2020214046,      2015716109,      2011142577,      2006493621,      2001769416,      1996970140,      1992095971,      1987147093,
    1982123692,      1977025955,      1971854075,      1966608244,      1961288660,      1955895523,      1950429033,      1944889398,
    1939276823,      1933591520,      1927833702,      1922003585,      1916101387,      1910127330,      1904081639,      1897964538,
    1891776259,      1885517033,      1879187094,      1872786681,      1866316033,      1859775393,      1853165007,      1846485121,
    1839735987,      1832917858,      1826030989,      1819075639,      1812052069,      1804960541,      1797801323,      1790574682,
    1783280889,      1775920218,      1768492946,      1760999350,      1753439711,      1745814314,      1738123443,      1730367389,
    1722546440,      1714660891,      1706711038,      1698697178,      1690619613,      1682478644,      1674274578,      1666007722,
    1657678385,      1649286882,      1640833525,      1632318633,      1623742524,      1615105520,      1606407945,      1597650125,
    1588832389,      1579955068,      1571018493,      1562023001,      1552968929,      1543856616,      1534686404,      1525458638,
    1516173662,      1506831825,      1497433478,      1487978973,      1478468665,      1468902910,      1459282067,      1449606497,
    1439876562,      1430092629,      1420255062,      1410364232,      1400420510,      1390424268,      1380375881,      1370275726,
    1360124182,      1349921629,      1339668451,      1329365031,      1319011757,      1308609016,      1298157198,      1287656696,
    1277107903,      1266511215,      1255867029,      1245175745,      1234437763,      1223653486,      1212823319,      1201947667,
    1191026940,      1180061545,      1169051895,      1157998402,      1146901480,      1135761547,      1124579020,      1113354317,
    1102087861,      1090780074,      1079431379,      1068042202,      1056612971,      1045144114,      1033636061,      1022089244,
    1010504095,       998881050,       987220544,       975523015,       963788901,       952018642,       940212679,       928371456,
    916495417,       904585006,       892640671,       880662859,       868652020,       856608604,       844533063,       832425849,
    820287418,       808118223,       795918721,       783689371,       771430629,       759142957,       746826815,       734482665,
    722110970,       709712193,       697286801,       684835258,       672358032,       659855590,       647328402,       634776938,
    622201668,       609603063,       596981596,       584337741,       571671972,       558984763,       546276591,       533547932,
    520799264,       508031064,       495243811,       482437986,       469614067,       456772537,       443913877,       431038568,
    418147095,       405239940,       392317587,       379380521,       366429227,       353464191,       340485899,       327494838,
    314491496,       301476358,       288449915,       275412654,       262365064,       249307635,       236240856,       223165217,
    210081209,       196989322,       183890048,       170783877,       157671302,       144552813,       131428903,       118300065,
    105166789,        92029570,        78888899,        65745270,        52599175,        39451107,        26301560,        13151027,
};
#endif