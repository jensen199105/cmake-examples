#ifndef __MEM3_H__
#define __MEM3_H__

#include "AISP_TSL_base.h"

typedef U32 u32;
typedef U16 u16;
typedef S16 i16;
typedef U8 u8;
typedef S8 i8;

#define ROUNDUP8(val)   (((val) + 7) & (~7))

S32 memsys3Size(VOID *p);
VOID *memsys3Malloc(S32 nBytes);
VOID memsys3Free(VOID *pPrior);
S32 memsys3Init(VOID *pHeap, u32 size);

#endif
