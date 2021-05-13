/*
** 2007 October 14
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** This file contains the C functions that implement a memory
** allocation subsystem for use by SQLite.
**
** This version of the memory allocation subsystem omits all
** use of malloc(). The SQLite user supplies a block of memory
** before calling sqlite3_initialize() from which allocations
** are made and returned by the xMalloc() and xRealloc()
** implementations. Once sqlite3_initialize() has been called,
** the amount of memory available to SQLite is fixed and cannot
** be changed.
**
** This version of the memory allocation subsystem is included
** in the build only if SQLITE_ENABLE_MEMSYS3 is defined.
*/
#include "mem3.h"

/*
 * assert is not allowed to be used in lite-sspe
 * so redefine assert to blank
 */
#ifdef assert
#undef assert
#endif
#define assert(expr)

/*
** This version of the memory allocator is only built into the library
** SQLITE_ENABLE_MEMSYS3 is defined. Defining this symbol does not
** mean that the library will use a memory-pool by default, just that
** it is available. The mempool allocator is activated by calling
** sqlite3_config().
*/

/*
** Maximum size (in Mem3Blocks) of a "small" chunk.
*/
#define MX_SMALL 10


/*
** Number of freelist hash slots
*/
#define N_HASH  61

/*
** A memory allocation (also called a "chunk") consists of two or
** more blocks where each block is 8 bytes.  The first 8 bytes are
** a header that is not returned to the user.
**
** A chunk is two or more blocks that is either checked out or
** free.  The first block has format u.hdr.  u.hdr.size4x is 4 times the
** size of the allocation in blocks if the allocation is free.
** The u.hdr.size4x&1 bit is true if the chunk is checked out and
** false if the chunk is on the freelist.  The u.hdr.size4x&2 bit
** is true if the previous chunk is checked out and false if the
** previous chunk is free.  The u.hdr.prevSize field is the size of
** the previous chunk in blocks if the previous chunk is on the
** freelist. If the previous chunk is checked out, then
** u.hdr.prevSize can be part of the data for that chunk and should
** not be read or written.
**
** We often identify a chunk by its index in mem3.aPool[].  When
** this is done, the chunk index refers to the second block of
** the chunk.  In this way, the first chunk has an index of 1.
** A chunk index of 0 means "no such chunk" and is the equivalent
** of a NULL pointer.
**
** The second block of free chunks is of the form u.list.  The
** two fields form a double-linked list of chunks of related sizes.
** Pointers to the head of the list are stored in mem3.aiSmall[]
** for smaller chunks and mem3.aiHash[] for larger chunks.
**
** The second block of a chunk is user data if the chunk is checked
** out.  If a chunk is checked out, the user data may extend into
** the u.hdr.prevSize value of the following chunk.
*/
typedef struct Mem3Block Mem3Block;
struct Mem3Block
{
    union
    {
        struct
        {
            /*
               The u.hdr.prevSize field is the size of
               ** the previous chunk in blocks if the previous chunk is on the
               ** freelist. If the previous chunk is checked out, then
               ** u.hdr.prevSize can be part of the data for that chunk and should
               ** not be read or written.
            */
            u32 prevSize;   /* Size of previous chunk in Mem3Block elements */
            /* The u.hdr.size4x&1 bit is true if the chunk is checked out */
            /* The u.hdr.size4x&2 bit is true if the previous chunk is checked out*/
            S32 size4x;     /* 4x the size of current chunk in Mem3Block elements */
        } hdr;
        struct
        {
            u32 next;       /* Index in mem3.aPool[] of next free chunk */
            u32 prev;       /* Index in mem3.aPool[] of previous free chunk */
        } list;
    } u;
};

/*
** All of the STATIC variables used by this module are collected
** into a single structure named "mem3".  This is to keep the
** STATIC variables organized and to reduce namespace pollution
** when this module is combined with other in the amalgamation.
*/
STATIC struct Mem3Global
{
    /*
    ** Memory available for allocation. nPool is the size of the array
    ** (in Mem3Blocks) pointed to by aPool less 2.
    */
    u32 nPool;
    Mem3Block *aPool;

    /*
    ** True if we are evaluating an out-of-memory callback.
    */
    S32 alarmBusy;

    /*
    ** The minimum amount of free space that we have seen.
    */
    S32 mnMaster;

    /*
    ** iMaster is the index of the master chunk.  Most new allocations
    ** occur off of this chunk.  szMaster is the size (in Mem3Blocks)
    ** of the current master.  iMaster is 0 if there is not master chunk.
    ** The master chunk is not in either the aiHash[] or aiSmall[].
    */
    u32 iMaster;
    S32 szMaster;

    /*
    ** Array of lists of free blocks according to the block size
    ** for smaller chunks, or a hash on the block size for larger
    ** chunks.
    */
    u32 aiSmall[MX_SMALL - 1]; /* For sizes 2 through MX_SMALL, inclusive */
    u32 aiHash[N_HASH];        /* For sizes MX_SMALL+1 and larger */
} mem3;

/*#define mem3 GLOBAL(struct Mem3Global, mem3)*/

/*
** Unlink the chunk at mem3.aPool[i] from list it is currently
** on.  *pRoot is the list that i is a member of.
*/
STATIC VOID memsys3UnlinkFromList(u32 i, u32 *pRoot)
{
    u32 next = mem3.aPool[i].u.list.next;
    u32 prev = mem3.aPool[i].u.list.prev;

    /*assert( sqlite3_mutex_held(mem3.mutex) );*/
    if (prev == 0)
    {
        *pRoot = next;
    }
    else
    {
        mem3.aPool[prev].u.list.next = next;
    }

    if (next)
    {
        mem3.aPool[next].u.list.prev = prev;
    }

    mem3.aPool[i].u.list.next = 0;
    mem3.aPool[i].u.list.prev = 0;
}

/*
** Unlink the chunk at index i from
** whatever list is currently a member of.
*/
STATIC VOID memsys3Unlink(u32 i)
{
    u32 size = 0;
    u32 hash = 0;

    /*assert( sqlite3_mutex_held(mem3.mutex) );*/
    assert((mem3.aPool[i - 1].u.hdr.size4x & 1) == 0);
    assert(i >= 1);
    size = mem3.aPool[i - 1].u.hdr.size4x / 4;
    assert(size == mem3.aPool[i + size - 1].u.hdr.prevSize);
    assert(size >= 2);

    if (size <= MX_SMALL)
    {
        memsys3UnlinkFromList(i, &mem3.aiSmall[size - 2]);
    }
    else
    {
        hash = size % N_HASH;
        memsys3UnlinkFromList(i, &mem3.aiHash[hash]);
    }
}

/*
** Link the chunk at mem3.aPool[i] so that is on the list rooted
** at *pRoot.
*/
STATIC VOID memsys3LinkIntoList(u32 i, u32 *pRoot)
{
    /*assert( sqlite3_mutex_held(mem3.mutex) );*/
    mem3.aPool[i].u.list.next = *pRoot;
    mem3.aPool[i].u.list.prev = 0;

    if (*pRoot)
    {
        mem3.aPool[*pRoot].u.list.prev = i;
    }

    *pRoot = i;
}

/*
** Link the chunk at index i into either the appropriate
** small chunk list, or into the large chunk hash table.
*/
STATIC VOID memsys3Link(u32 i)
{
    u32 size = 0;
    u32 hash = 0;

    /*assert( sqlite3_mutex_held(mem3.mutex) );*/
    assert(i >= 1);
    assert((mem3.aPool[i - 1].u.hdr.size4x & 1) == 0);
    size = mem3.aPool[i - 1].u.hdr.size4x / 4;
    assert(size == mem3.aPool[i + size - 1].u.hdr.prevSize);
    assert(size >= 2);

    if (size <= MX_SMALL)
    {
        memsys3LinkIntoList(i, &mem3.aiSmall[size - 2]);
    }
    else
    {
        hash = size % N_HASH;
        memsys3LinkIntoList(i, &mem3.aiHash[hash]);
    }
}

/*
** Called when we are unable to satisfy an allocation of nBytes.
*/
STATIC VOID memsys3OutOfMemory(S32 nByte)
{
    /*TODO*/
#if 0
    if (!mem3.alarmBusy)
    {
        mem3.alarmBusy = 1;
        /*assert( sqlite3_mutex_held(mem3.mutex) );*/
        sqlite3_mutex_leave(mem3.mutex);
        sqlite3_release_memory(nByte);
        sqlite3_mutex_enter(mem3.mutex);
        mem3.alarmBusy = 0;
    }

#endif
}


/*
** Chunk i is a free chunk that has been unlinked.  Adjust its
** size parameters for check-out and return a pointer to the
** user portion of the chunk.
*/
STATIC VOID *memsys3Checkout(u32 i, u32 nBlock)
{
    u32 x = 0;
    /*assert( sqlite3_mutex_held(mem3.mutex) );*/
    assert(i >= 1);
    assert(mem3.aPool[i - 1].u.hdr.size4x / 4 == nBlock);
    assert(mem3.aPool[i + nBlock - 1].u.hdr.prevSize == nBlock);
    x = mem3.aPool[i - 1].u.hdr.size4x;
    mem3.aPool[i - 1].u.hdr.size4x = nBlock * 4 | 1 | (x & 2);
    mem3.aPool[i + nBlock - 1].u.hdr.prevSize = nBlock;
    mem3.aPool[i + nBlock - 1].u.hdr.size4x |= 2;
    return &mem3.aPool[i];
}

/*
** Carve a piece off of the end of the mem3.iMaster free chunk.
** Return a pointer to the new allocation.  Or, if the master chunk
** is not large enough, return 0.
*/
STATIC VOID *memsys3FromMaster(S32 nBlock)
{
    u32 newi = 0;
    u32 x = 0;

    /*assert( sqlite3_mutex_held(mem3.mutex) );*/
    assert(mem3.szMaster >= nBlock);

    if (nBlock >= mem3.szMaster - 1)
    {
        /* Use the entire master */
        VOID *p = memsys3Checkout(mem3.iMaster, mem3.szMaster);
        mem3.iMaster = 0;
        mem3.szMaster = 0;
        mem3.mnMaster = 0;
        return p;
    }

    newi = mem3.iMaster + mem3.szMaster - nBlock;
    assert(newi > mem3.iMaster + 1);
    mem3.aPool[mem3.iMaster + mem3.szMaster - 1].u.hdr.prevSize = nBlock;
    mem3.aPool[mem3.iMaster + mem3.szMaster - 1].u.hdr.size4x |= 2; /*previous chunk is alloced*/
    mem3.aPool[newi - 1].u.hdr.size4x = nBlock * 4 + 1; /*current chunk is alloced*/
    mem3.szMaster -= nBlock;    /*decrease master block count*/
    mem3.aPool[newi - 1].u.hdr.prevSize = mem3.szMaster;
    x = mem3.aPool[mem3.iMaster - 1].u.hdr.size4x & 2; /*get iMaster bit flag, alloced or not?*/
    mem3.aPool[mem3.iMaster - 1].u.hdr.size4x = mem3.szMaster * 4 | x;

    if (mem3.szMaster < mem3.mnMaster)
    {
        mem3.mnMaster = mem3.szMaster;
    }

    return (VOID *)&mem3.aPool[newi];
}

/*
** *pRoot is the head of a list of free chunks of the same size
** or same size hash.  In other words, *pRoot is an entry in either
** mem3.aiSmall[] or mem3.aiHash[].
**
** This routine examines all entries on the given list and tries
** to coalesce each entries with adjacent free chunks.
**
** If it sees a chunk that is larger than mem3.iMaster, it replaces
** the current mem3.iMaster with the new larger chunk.  In order for
** this mem3.iMaster replacement to work, the master chunk must be
** linked into the hash tables.  That is not the normal state of
** affairs, of course.  The calling routine must link the master
** chunk before invoking this routine, then must unlink the (possibly
** changed) master chunk once this routine has finished.
*/
STATIC VOID memsys3Merge(u32 *pRoot)
{
    u32 iNext = 0;
    u32 prev = 0;
    S32 size = 0;
    u32 i = 0;
    u32 x = 0;

    /*assert( sqlite3_mutex_held(mem3.mutex) );*/
    for (i = *pRoot; i > 0; i = iNext)
    {
        iNext = mem3.aPool[i].u.list.next;
        size = mem3.aPool[i - 1].u.hdr.size4x;
        assert((size & 1) == 0);

        if ((size & 2) == 0)
        {
            memsys3UnlinkFromList(i, pRoot);
            assert(i > mem3.aPool[i - 1].u.hdr.prevSize);
            prev = i - mem3.aPool[i - 1].u.hdr.prevSize;

            if (prev == iNext)
            {
                iNext = mem3.aPool[prev].u.list.next;
            }

            memsys3Unlink(prev);
            size = i + size / 4 - prev;
            x = mem3.aPool[prev - 1].u.hdr.size4x & 2;
            mem3.aPool[prev - 1].u.hdr.size4x = size * 4 | x;
            mem3.aPool[prev + size - 1].u.hdr.prevSize = size;
            memsys3Link(prev);
            i = prev;
        }
        else
        {
            size /= 4;
        }

        if (size > mem3.szMaster)
        {
            mem3.iMaster = i;
            mem3.szMaster = size;
        }
    }
}

/*
** Return a block of memory of at least nBytes in size.
** Return NULL if unable.
**
** This function assumes that the necessary mutexes, if any, are
** already held by the caller. Hence "Unsafe".
*/
STATIC VOID *memsys3MallocUnsafe(S32 nByte)
{
    u32 i = 0;
    S32 nBlock = 0;
    u32 toFree = 0;
    /*assert( sqlite3_mutex_held(mem3.mutex) );*/
    assert(sizeof(Mem3Block) == 8);

    if (nByte <= 12)
    {
        nBlock = 2;
    }
    else
    {
        nBlock = (nByte + 11) / 8;
    }

    assert(nBlock >= 2);

    /* STEP 1:
    ** Look for an entry of the correct size in either the small
    ** chunk table or in the large chunk hash table.  This is
    ** successful most of the time (about 9 times out of 10).
    */
    if (nBlock <= MX_SMALL)
    {
        i = mem3.aiSmall[nBlock - 2];

        if (i > 0)
        {
            memsys3UnlinkFromList(i, &mem3.aiSmall[nBlock - 2]);
            return memsys3Checkout(i, nBlock);
        }
    }
    else
    {
        S32 hash = nBlock % N_HASH;

        for (i = mem3.aiHash[hash]; i > 0; i = mem3.aPool[i].u.list.next)
        {
            if (mem3.aPool[i - 1].u.hdr.size4x / 4 == nBlock)
            {
                memsys3UnlinkFromList(i, &mem3.aiHash[hash]);
                return memsys3Checkout(i, nBlock);
            }
        }
    }

    /* STEP 2:
    ** Try to satisfy the allocation by carving a piece off of the end
    ** of the master chunk.  This step usually works if step 1 fails.
    */
    if (mem3.szMaster >= nBlock)
    {
        return memsys3FromMaster(nBlock);
    }

    /* STEP 3:
    ** Loop through the entire memory pool.  Coalesce adjacent free
    ** chunks.  Recompute the master chunk as the largest free chunk.
    ** Then try again to satisfy the allocation by carving a piece off
    ** of the end of the master chunk.  This step happens very
    ** rarely (we hope!)
    */

    /*the head and tail are alloced, but middle is free, when master used up, we will step into this routine*/
    for (toFree = nBlock * 16; toFree < (mem3.nPool * 16); toFree *= 2)
    {
        memsys3OutOfMemory(toFree);

        if (mem3.iMaster)
        {
            memsys3Link(mem3.iMaster);
            mem3.iMaster = 0;
            mem3.szMaster = 0;
        }

        for (i = 0; i < N_HASH; i++)
        {
            memsys3Merge(&mem3.aiHash[i]);
        }

        for (i = 0; i < MX_SMALL - 1; i++)
        {
            memsys3Merge(&mem3.aiSmall[i]);
        }

        if (mem3.szMaster)
        {
            memsys3Unlink(mem3.iMaster);

            if (mem3.szMaster >= nBlock)
            {
                return memsys3FromMaster(nBlock);
            }
        }
    }

    /* If none of the above worked, then we fail. */
    return 0;
}

/*
** Free an outstanding memory allocation.
**
** This function assumes that the necessary mutexes, if any, are
** already held by the caller. Hence "Unsafe".
*/
STATIC VOID memsys3FreeUnsafe(VOID *pOld)
{
    Mem3Block *p = (Mem3Block *)pOld;
    S32 i = 0;
    u32 size = 0;
    u32 x = 0;

    /*assert( sqlite3_mutex_held(mem3.mutex) );*/
    assert(p > mem3.aPool && p < &mem3.aPool[mem3.nPool]);
    i = (S32)(p - mem3.aPool);
    assert((mem3.aPool[i - 1].u.hdr.size4x & 1) == 1);
    size = mem3.aPool[i - 1].u.hdr.size4x / 4;
    assert(i + size <= mem3.nPool + 1);
    mem3.aPool[i - 1].u.hdr.size4x &= ~1; /*current chunk is free*/
    mem3.aPool[i + size - 1].u.hdr.prevSize = size;
    mem3.aPool[i + size - 1].u.hdr.size4x &= ~2; /*previous chunk is free*/
    memsys3Link(i);

    /* Try to expand the master using the newly freed chunk */
    if (mem3.iMaster)
    {
        while ((mem3.aPool[mem3.iMaster - 1].u.hdr.size4x & 2) == 0) /*previous chunk is free, merge previous chunks*/
        {
            size = mem3.aPool[mem3.iMaster - 1].u.hdr.prevSize;
            mem3.iMaster -= size;
            mem3.szMaster += size;
            memsys3Unlink(mem3.iMaster);    /*unlink from master block and expand master chunk*/
            x = mem3.aPool[mem3.iMaster - 1].u.hdr.size4x & 2;
            mem3.aPool[mem3.iMaster - 1].u.hdr.size4x = mem3.szMaster * 4 | x;
            mem3.aPool[mem3.iMaster + mem3.szMaster - 1].u.hdr.prevSize = mem3.szMaster;
        }

        x = mem3.aPool[mem3.iMaster - 1].u.hdr.size4x & 2;

        while ((mem3.aPool[mem3.iMaster + mem3.szMaster - 1].u.hdr.size4x & 1) == 0) /*last chunk is free, try to merge*/
        {
            memsys3Unlink(mem3.iMaster + mem3.szMaster);
            mem3.szMaster += mem3.aPool[mem3.iMaster + mem3.szMaster - 1].u.hdr.size4x / 4;
            mem3.aPool[mem3.iMaster - 1].u.hdr.size4x = mem3.szMaster * 4 | x;
            mem3.aPool[mem3.iMaster + mem3.szMaster - 1].u.hdr.prevSize = mem3.szMaster;
        }
    }
}

/*
** Return the size of an outstanding allocation, in bytes.  The
** size returned omits the 8-byte header overhead.  This only
** works for chunks that are currently checked out.
*/
S32 memsys3Size(VOID *p)
{
    Mem3Block *pBlock = NULL;

    if (NULL == p)
    {
        return 0;
    }

    pBlock = (Mem3Block *)p;
    assert((pBlock[-1].u.hdr.size4x & 1) != 0);
    return (S32)(pBlock[-1].u.hdr.size4x & ~3) * 2 - 4;
}

/*
** Allocate nBytes of memory.
*/
VOID *memsys3Malloc(S32 nBytes)
{
    VOID *p = NULL;
    assert(nBytes > 0);          /* malloc.c filters out 0 byte requests */
    p = memsys3MallocUnsafe(nBytes);
    return p;
}

/*
** Free memory.
*/
VOID memsys3Free(VOID *pPrior)
{
    assert(pPrior);
    memsys3FreeUnsafe(pPrior);
}

/*
** Initialize this module.
*/
S32 memsys3Init(VOID *pHeap, u32 size)
{
    /*make sure pHeap should be 8 bytes round*/
    VOID *_pHeap = (VOID *)ROUNDUP8(pHeap - NULL);
    size -= _pHeap - pHeap;
    /* Store a pointer to the memory block in global structure mem3. */
    assert(sizeof(Mem3Block) == 8);
    mem3.aPool = (Mem3Block *)_pHeap;
    mem3.nPool = (size / sizeof(Mem3Block)) - 2;
    /* Initialize the master block. */
    mem3.szMaster = mem3.nPool;
    mem3.mnMaster = mem3.szMaster;
    mem3.iMaster = 1;
    mem3.aPool[0].u.hdr.size4x = (mem3.szMaster << 2) + 2;
    mem3.aPool[mem3.nPool].u.hdr.prevSize = mem3.nPool;
    mem3.aPool[mem3.nPool].u.hdr.size4x = 1;
    return 0;
}
