// uncomment one to use
//#define INTERNAL_IMPL
//#define DLSYM_IMPL
#define HOOK_IMPL

#if defined(DEBUG)
#   if defined(__GNUC__)
#       if defined(INTERNAL_IMPL)

#include <stdio.h>
#include <stdlib.h>

static size_t s_MallocCallCount = 0;
static size_t s_FreeCallCount = 0;

// internal glibc malloc and free. Same as normal malloc and free.
extern void* __libc_malloc(size_t);
extern void  __libc_free(void*);

void MemoryReporterInit() __attribute__((constructor));
void MemoryReporterEnd() __attribute__((destructor));

void* malloc(size_t size)
{
    void* ptr = __libc_malloc(size);
    if (ptr) 
        s_MallocCallCount++;
    return ptr;
}

void free(void* block)
{
    s_FreeCallCount++;
    __libc_free(block);
}

void MemoryReporterInit()
{
    puts("[MemoryReport] Initialized.\n");
}

void MemoryReporterEnd()
{
    printf("[MemoryReport] malloc calls: %lu, free calls: %lu, diff: %lu\n",
        s_MallocCallCount, s_FreeCallCount, s_MallocCallCount - s_FreeCallCount);
}

#       elif defined(DLSYM_IMPL)

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

static size_t s_MallocCallCount = 0;
static size_t s_FreeCallCount = 0;

typedef void* MallocFuncType(size_t);
typedef void  FreeFuncType(void*);

// internal glibc malloc and free. Same as normal malloc and free.
static MallocFuncType* s_glibc_malloc;
static FreeFuncType*   s_glibc_free;

void MemoryReporterInit() __attribute__((constructor));
void MemoryReporterEnd() __attribute__((destructor));

void* malloc(size_t size)
{
    void* ptr = s_glibc_malloc(size);
    if (ptr) 
        s_MallocCallCount++;
    return ptr;
}

void free(void* block)
{
    s_FreeCallCount++;
    s_glibc_free(block);
}

void MemoryReporterInit()
{
    s_glibc_malloc = (MallocFuncType*)dlsym(RTLD_NEXT, "malloc");
    s_glibc_free = (FreeFuncType*)dlsym(RTLD_NEXT, "free");
    
    if (s_glibc_malloc && s_glibc_free)
    {
        puts("[MemoryReport] Initialized.");
    }
    else
    {
        perror("[MemoryReport] Failed to initialize");
        exit(EXIT_FAILURE);
    }
}

void MemoryReporterEnd()
{
    printf("[MemoryReport] malloc calls: %lu, free calls: %lu, diff: %lu\n",
        s_MallocCallCount, s_FreeCallCount, s_MallocCallCount - s_FreeCallCount);
}

#       elif defined(HOOK_IMPL)

#include <malloc.h>
#include <stdio.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations" // hooks are deprecated

static size_t s_MallocCallCount = 0;
static size_t s_FreeCallCount = 0;

void  MemoryReporterInit() __attribute__((constructor));
void  MemoryReporterEnd() __attribute__((destructor));
static void* MallocHook(size_t size, const void* caller);
static void  FreeHook(void* block, const void* caller);

static void* (*OldMallocHook)(size_t, const void*) = NULL;
static void  (*OldFreeHook)(void*, const void*) = NULL;

void MemoryReporterInit()
{
    OldMallocHook = __malloc_hook;
    OldFreeHook = __free_hook;
    __malloc_hook = MallocHook;
    __free_hook = FreeHook;
    puts("[MemoryReport] Initialized.");
}

void MemoryReporterEnd()
{
    printf("[MemoryReport] malloc calls: %lu, free calls: %lu, diff: %lu\n",
        s_MallocCallCount, s_FreeCallCount, s_MallocCallCount - s_FreeCallCount);
}

static void* MallocHook(size_t size, const void* caller)
{
    void *result;
    __malloc_hook = OldMallocHook;
    result = malloc(size);
    OldMallocHook = __malloc_hook;
    __malloc_hook = MallocHook;

    if (result) s_MallocCallCount++;

    return result;
}

static void FreeHook(void* block, const void* caller)
{
    __free_hook = OldFreeHook;
    free(block);
    OldFreeHook = __free_hook;
    __free_hook = FreeHook;

    s_FreeCallCount++;
}

#pragma GCC diagnostic pop

#       endif // INTERNAL_IMPL or HOOK_IMPL
#   else
#warning Compile with GCC to get malloc overrides
#   endif // __GNUC__
#endif // DEBUG