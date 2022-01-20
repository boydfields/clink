// Copyright (c) 2022 Christopher Antos
// License: http://opensource.org/licenses/MIT

#pragma once

#ifdef DEBUG
#define USE_MEMORY_TRACKING
#define INCLUDE_CALLSTACKS
#endif

#include <malloc.h>

#ifdef USE_MEMORY_TRACKING

#ifdef __cplusplus

_Ret_notnull_ _Post_writable_byte_size_(size)
void* _cdecl operator new(size_t size);
void _cdecl operator delete(void* size);

_Ret_notnull_ _Post_writable_byte_size_(size)
void* _cdecl operator new[](size_t size);
void _cdecl operator delete[](void* size);

#ifndef __PLACEMENT_NEW_INLINE
#   define __PLACEMENT_NEW_INLINE
inline void* _cdecl operator new(size_t, void* size) { return size; }
#   if defined(_MSC_VER)
#       if _MSC_VER >= 1200
inline void _cdecl operator delete(void*, void*) { return; }
#       endif
#   endif
#endif

#endif // __cplusplus

enum
{
#ifdef INCLUDE_CALLSTACKS
    MAX_STACK_DEPTH             = 12,
#endif

    memNew                      = 0x00000001,
    memNewArray                 = 0x00000002,
#ifdef USE_RTTI
    memRTTI                     = 0x00000100,
#endif
    memNoSizeCheck              = 0x00000200,
    memNoStack                  = 0x00000400,
    memSkipOneFrame             = 0x00010000,
    memSkipAnotherFrame         = 0x00020000,
    memZeroInit                 = 0x00040000,

    memAllocatorMask            = 0x000000ff,
    memImmutableMask            = 0x0000ffff,
};

#define _MEM_0                  , 0
#define _MEM_NEW                , memNew
#define _MEM_NEWARRAY           , memNewArray
#ifdef USE_RTTI
#define _MEM_RTTI               , memRTTI|memNew
#endif
#define _MEM_NOSIZECHECK        , memNoSizeCheck

#define memalloc                dbgalloc
#define memcalloc               dbgcalloc
#define memrealloc              dbgrealloc
#define memfree                 dbgfree

#ifdef __cplusplus
extern "C" {
#endif

char const* dbginspectmemory(void const* pv, size_t size);

void dbgsetsanealloc(size_t maxalloc, size_t maxrealloc, size_t const* exceptions);

void* dbgalloc_(size_t size, unsigned int flags);
void* dbgrealloc_(void* pv, size_t size, unsigned int flags);
void dbgfree_(void* pv, unsigned int type);

void dbgsetlabel(void* pv, char const* label, bool copy);
void dbgdeadfillpointer(void** ppv);

size_t dbggetallocnumber();
size_t dbgcchcopy(char const* from, char* to, size_t max);
void dbgcheck();
void dbgchecksince(size_t alloc_number);

#ifdef USE_HEAP_STATS
// TODO: report heap stats
#endif

#ifdef __cplusplus
}
#endif

#else // !USE_MEMORY_TRACKING

#define _MEM_0
#define _MEM_NEW
#define _MEM_NEWARRAY
#define _MEM_OBJECT
#define _MEM_NOSIZECHECK

#endif // !USE_MEMORY_TRACKING