/**
 * Heap base abstract class
 */

#ifndef __COMMON_HEAP_H__
#define __COMMON_HEAP_H__

#ifdef __cplusplus
extern "C" {
#endif

#pragma once

/**
 * 'Magic' - aka a sentinel which will be added as a guard band for each allocation in debug mode
 */
#define ALLOCATION_HEADER_MAGIC "__HEADER_MAGIC_GUARD_BAND__"
#define ALLOCATION_FOOTER_MAGIC "__FOOTER_MAGIC_GUARD_BAND__"

// Ensure it's more than the actual header and footer size
#define ALLOCATION_HEADER_MAGIC_SIZE        32
#define ALLOCATION_FOOTER_MAGIC_SIZE        32

/**
 * The internal base structure representing the allocation header for
 * all of the heap implementations.
 *
 * IMPORTANT!!! Ensure this structure is tightly packed without tight packing directives
 */
typedef struct
{
    UINT64 size;
    UINT32 type;
    union {
        UINT32 vramHandle;
        UINT32 flags;
    };

#ifdef HEAP_DEBUG
    BYTE magic[ALLOCATION_HEADER_MAGIC_SIZE];
#endif
} ALLOCATION_HEADER, *PALLOCATION_HEADER;

typedef struct
{
    UINT64 size;
#ifdef HEAP_DEBUG
    BYTE magic[ALLOCATION_FOOTER_MAGIC_SIZE];
#endif
} ALLOCATION_FOOTER, *PALLOCATION_FOOTER;

/**
 * The following macros are used to access various fields without de0referencing the structure
 * which would cause unaligned access in the generated code.
 */
#define GET_AIV_HEADER_SIZE_PTR(p)              ((PBYTE)(PALLOCATION_HEADER)(p) + 0)
#define GET_AIV_HEADER_TYPE_PTR(p)              (GET_AIV_HEADER_SIZE_PTR(p) + SIZEOF(UINT64))
#define GET_AIV_HEADER_VRAM_HANDLE_PTR(p)       (GET_AIV_HEADER_TYPE_PTR(p) + SIZEOF(UINT32))
#define GET_AIV_HEADER_FLAGS_PTR(p)             (GET_AIV_HEADER_TYPE_PTR(p) + SIZEOF(UINT32))
#define GET_AIV_HEADER_MAGIC_PTR(p)             (GET_AIV_HEADER_FLAGS_PTR(p) + SIZEOF(UINT32))

#define GET_AIV_FOOTER_SIZE_PTR(p)              ((PBYTE)(PALLOCATION_FOOTER)(p) + 0)
#define GET_AIV_FOOTER_MAGIC_PTR(p)             (GET_AIV_FOOTER_SIZE_PTR(p) + SIZEOF(UINT64))

/**
 * Allocate a buffer from the heap
 */
DEFINE_HEAP_ALLOC(commonHeapAlloc);

/**
 * Free the previously allocated buffer handle
 */
DEFINE_HEAP_FREE(commonHeapFree);

/**
 * Gets the heap size
 */
DEFINE_GET_HEAP_SIZE(commonHeapGetSize);

/**
 * Gets the allocation size
 */
DEFINE_HEAP_GET_ALLOC_SIZE(commonHeapGetAllocSize);

/**
 * Sets the allocation size
 */
DEFINE_HEAP_SET_ALLOC_SIZE(commonHeapSetAllocSize);

/**
 * Maps the allocation handle to memory. This is needed for in-direct allocation on vRAM
 */
DEFINE_HEAP_MAP(commonHeapMap);

/**
 * Un-maps the previously mapped buffer
 */
DEFINE_HEAP_UNMAP(commonHeapUnmap);

/**
 * Release the entire heap
 */
DEFINE_RELEASE_HEAP(commonHeapRelease);

/**
 * Initialize the heap with a given limit
 */
DEFINE_INIT_HEAP(commonHeapInit);

/**
 * Debug/check heap
 */
DEFINE_HEAP_CHK(commonHeapDebugCheckAllocator);

/**
 * Increment the allocations/count
 */
VOID incrementUsage(PHeap, UINT64);

/**
 * Decrenents the allocations/count
 */
VOID decrementUsage(PHeap, UINT64);

/**
 * Creates the heap object itself
 */
STATUS commonHeapCreate(PHeap*, UINT32);

/**
 * Validate heap
 */
STATUS validateHeap(PHeap);

#ifdef __cplusplus
}
#endif

#endif // __COMMON_HEAP_H__
