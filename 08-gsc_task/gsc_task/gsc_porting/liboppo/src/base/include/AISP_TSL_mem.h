#ifndef __AISP_TSL_MEM_H__
#define __AISP_TSL_MEM_H__

#ifndef __AISP_TSL_BASE_H__
#error "forbid to include this header directly"
#endif

//TODO: rename & refactor
#define AISP_ALIGN_8_MASK               (7)
#define AISP_ALIGN_8_SIZE               (8)
#define AISP_ALIGN_SIZE(size, mask)     (((size) + (mask)) & ~(mask))
#define AISP_MEM_ALIGN(addr, align)     ((VOID*)(addr) + ((align) - ((VOID*)(addr) - NULL) % (align)))

typedef enum
{
    MOD_AEC,
    MOD_WAKEUP,
    MOD_MAX,        /*max number of algorithm used in one system*/
} MODULE_E;

S32 AISP_TSL_meminit(VOID *pHeapStart, S32 iSize);
VOID *AISP_TSL_malloc(S32 iSize, MODULE_E enMod);
VOID *AISP_TSL_calloc(S32 iMemb, S32 iSize, MODULE_E enMod);
VOID AISP_TSL_free(VOID *pAddr, MODULE_E enMod);
S32 AISP_TSL_memusage(MODULE_E enMod);

VOID *AISP_TSL_memset(VOID *s, S32 c, S32 count);
VOID *AISP_TSL_memcpy(VOID *dest, CONST VOID *src, S32 count);
VOID *AISP_TSL_memmove(VOID *dest, CONST VOID *src, S32 count);
S32 AISP_TSL_memcmp(CONST VOID *cs, CONST VOID *ct, S32 count);

#endif
