#include "AISP_TSL_base.h"

#ifdef HANNING_SQRT_256
/* Hanning 256 window is W32Q31 */
CONST S32 hanning_win_sqrt_256[] =
{
    26250392,        52496862,        78735488,       104962348,
    131173524,       157365099,       183533160,       209673796,
    235783101,       261857174,       287892118,       313884044,
    339829067,       365723310,       391562905,       417343989,
    443062712,       468715229,       494297707,       519806324,
    545237269,       570586740,       595850950,       621026125,
    646108501,       671094332,       695979883,       720761436,
    745435289,       769997753,       794445160,       818773855,
    842980203,       867060588,       891011410,       914829092,
    938510074,       962050818,       985447805,      1008697540,
    1031796549,      1054741380,      1077528605,      1100154818,
    1122616639,      1144910711,      1167033702,      1188982308,
    1210753248,      1232343269,      1253749145,      1274967678,
    1295995697,      1316830059,      1337467652,      1357905391,
    1378140223,      1398169124,      1417989102,      1437597194,
    1456990471,      1476166034,      1495121019,      1513852593,
    1532357957,      1550634346,      1568679029,      1586489310,
    1604062526,      1621396053,      1638487300,      1655333713,
    1671932776,      1688282007,      1704378964,      1720221241,
    1735806471,      1751132325,      1766196514,      1780996786,
    1795530929,      1809796772,      1823792183,      1837515071,
    1850963386,      1864135117,      1877028296,      1889640998,
    1901971336,      1914017470,      1925777598,      1937249964,
    1948432853,      1959324594,      1969923560,      1980228166,
    1990236874,      1999948188,      2009360655,      2018472871,
    2027283473,      2035791145,      2043994616,      2051892659,
    2059484094,      2066767788,      2073742652,      2080407643,
    2086761766,      2092804071,      2098533655,      2103949662,
    2109051284,      2113837756,      2118308365,      2122462442,
    2126299367,      2129818566,      2133019513,      2135901731,
    2138464787,      2140708300,      2142631934,      2144235402,
    2145518464,      2146480928,      2147122651,      2147443536,
    2147443536,      2147122651,      2146480928,      2145518464,
    2144235402,      2142631934,      2140708300,      2138464787,
    2135901731,      2133019513,      2129818566,      2126299367,
    2122462442,      2118308365,      2113837756,      2109051284,
    2103949662,      2098533655,      2092804071,      2086761766,
    2080407643,      2073742652,      2066767788,      2059484094,
    2051892659,      2043994616,      2035791145,      2027283473,
    2018472871,      2009360655,      1999948188,      1990236874,
    1980228166,      1969923560,      1959324594,      1948432853,
    1937249964,      1925777598,      1914017470,      1901971336,
    1889640998,      1877028296,      1864135117,      1850963386,
    1837515071,      1823792183,      1809796772,      1795530929,
    1780996786,      1766196514,      1751132325,      1735806471,
    1720221241,      1704378964,      1688282007,      1671932776,
    1655333713,      1638487300,      1621396053,      1604062526,
    1586489310,      1568679029,      1550634346,      1532357957,
    1513852593,      1495121019,      1476166034,      1456990471,
    1437597194,      1417989102,      1398169124,      1378140223,
    1357905391,      1337467652,      1316830059,      1295995697,
    1274967678,      1253749145,      1232343269,      1210753248,
    1188982308,      1167033702,      1144910711,      1122616639,
    1100154818,      1077528605,      1054741380,      1031796549,
    1008697540,       985447805,       962050818,       938510074,
    914829092,       891011410,       867060588,       842980203,
    818773855,       794445160,       769997753,       745435289,
    720761436,       695979883,       671094332,       646108501,
    621026125,       595850950,       570586740,       545237269,
    519806324,       494297707,       468715229,       443062712,
    417343989,       391562905,       365723310,       339829067,
    313884044,       287892118,       261857174,       235783101,
    209673796,       183533160,       157365099,       131173524,
    104962348,        78735488,        52496862,        26250392,
};
#endif