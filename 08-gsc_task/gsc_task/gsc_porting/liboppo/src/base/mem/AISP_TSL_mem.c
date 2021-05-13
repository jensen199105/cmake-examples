#include "AISP_TSL_base.h"
#include "mem3/mem3.h"

typedef struct tagAispMemStat
{
    struct
    {
        U32 uiInuseCnt;
        U32 uiMaxCnt;
        U32 uiInuseSize;
        U32 uiMaxSize;
    } cnt[MOD_MAX];

} AISP_TSL_MEM_STAT_S;

typedef struct tagAispTslMem
{
    VOID *pHeapStart;
    U32 uiHeapSize;
    AISP_TSL_MEM_STAT_S stStat;
} AISP_TSL_MEM_GLOBAL_S;

AISP_TSL_MEM_GLOBAL_S g_stAispMemInfo = {0};

enum SSPE_MEM_STAT_TYPE
{
    MALLOC = 0,
    FREE,
};

#define SSPE_MEM_STAT(type, size, mod) \
{\
    if((type) == MALLOC ) { \
        g_stAispMemInfo.stStat.cnt[mod].uiInuseCnt++; \
        g_stAispMemInfo.stStat.cnt[mod].uiInuseSize += (size); \
        if(g_stAispMemInfo.stStat.cnt[mod].uiInuseSize > g_stAispMemInfo.stStat.cnt[mod].uiMaxSize) {\
            g_stAispMemInfo.stStat.cnt[mod].uiMaxSize = g_stAispMemInfo.stStat.cnt[mod].uiInuseSize; \
        }\
        if(g_stAispMemInfo.stStat.cnt[mod].uiInuseCnt > g_stAispMemInfo.stStat.cnt[mod].uiMaxCnt) {\
            g_stAispMemInfo.stStat.cnt[mod].uiMaxCnt = g_stAispMemInfo.stStat.cnt[mod].uiInuseCnt; \
        }\
        } else if((type) == FREE) { \
        g_stAispMemInfo.stStat.cnt[mod].uiInuseCnt--; \
        g_stAispMemInfo.stStat.cnt[mod].uiInuseSize -= (size); \
    }\
}\

VOID AISP_TSL_free(VOID *pAddr, MODULE_E enMod)
{
    if (NULL == pAddr)
    {
        return;
    }

    SSPE_MEM_STAT(FREE, memsys3Size(pAddr), enMod);
    return memsys3Free(pAddr);
}

VOID *AISP_TSL_malloc(S32 iSize, MODULE_E enMod)
{
    VOID *p = NULL;

    if (iSize == 0 || NULL == g_stAispMemInfo.pHeapStart)
    {
        return NULL;
    }

    p = memsys3Malloc(iSize);

    if (NULL != p)
    {
        SSPE_MEM_STAT(MALLOC, memsys3Size(p), enMod);
    }

    return p;
}

VOID *AISP_TSL_calloc(S32 iMemb, S32 iSize, MODULE_E enMod)
{
    VOID *p = NULL;
    U32 uiTotalSize = iMemb * iSize;
    p = AISP_TSL_malloc(uiTotalSize, enMod);

    if (NULL != p)
    {
        AISP_TSL_memset(p, 0, uiTotalSize);
    }

    return p;
}

S32 AISP_TSL_memusage(MODULE_E enMod)
{
    S32 i = 0;
    S32 iTotal = 0;

    if (enMod == MOD_MAX)
    {
        for (i = 0; i < MOD_MAX; i++)
        {
            iTotal += g_stAispMemInfo.stStat.cnt[i].uiInuseSize;
        }
    }
    else
    {
        iTotal = g_stAispMemInfo.stStat.cnt[enMod].uiInuseSize;
    }

    return iTotal;
}

S32 AISP_TSL_meminit(VOID *pHeapStart, S32 iSize)
{
    S32 iRet = 0;

    if (g_stAispMemInfo.pHeapStart == NULL)
    {
        AISP_TSL_memset(&g_stAispMemInfo, 0, sizeof(AISP_TSL_MEM_GLOBAL_S));
        g_stAispMemInfo.pHeapStart = pHeapStart;
        g_stAispMemInfo.uiHeapSize = iSize;

        iRet = memsys3Init(pHeapStart, iSize);
    }

    return iRet;
}

VOID *AISP_TSL_memset(VOID *s, S32 c, S32 count)
{
    S8 *xs = (S8 *) s;

    while (count--)
    {
        *xs++ = (S8)c;
    }

    return s;
}

VOID *AISP_TSL_memcpy(VOID *dest, CONST VOID *src, S32 count)
{
    S8 *tmp = (S8 *)dest;
    S8 *s = (S8 *)src;

    while (count--)
    {
        *tmp++ = *s++;
    }

    return dest;
}

VOID *AISP_TSL_memmove(VOID *dest, CONST VOID *src, S32 count)
{
    S8 *tmp = NULL;
    S8 *s = NULL;

    if (dest <= src)
    {
        tmp = (S8 *) dest;
        s = (S8 *) src;

        while (count--)
        {
            *tmp++ = *s++;
        }
    }
    else
    {
        tmp = (S8 *) dest + count;
        s = (S8 *) src + count;

        while (count--)
        {
            *--tmp = *--s;
        }
    }

    return dest;
}

S32 AISP_TSL_memcmp(CONST VOID *cs, CONST VOID *ct, S32 count)
{
    CONST U8 *su1 = NULL;
    CONST U8 *su2 = NULL;
    S8 res = 0;

    for (su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
    {
        if ((res = (U8)(*su1 - *su2)) != 0)
        {
            break;
        }
    }

    return res;
}
