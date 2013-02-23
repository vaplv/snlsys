#ifndef MEM_ALLOCATOR_H
#define MEM_ALLOCATOR_H

#include "snlsys.h"
#include <stddef.h>

/*******************************************************************************
 *
 * Memory allocator interface.
 *
 ******************************************************************************/
struct mem_allocator {
  void* (*alloc)
    (void* data,
     size_t size,
     const char* filename,
     unsigned int fileline);

  void* (*calloc)
    (void* data,
     size_t nbelmts,
     size_t size,
     const char* filename,
     unsigned int fileline);

  void* (*realloc)
    (void* data,
     void* mem,
     size_t size,
     const char* filename,
     unsigned int fileline);

  void* (*aligned_alloc)
    (void* data,
     size_t size,
     size_t alignment,
     const char* filename,
     unsigned int fileline);

  void (*free)
    (void* data,
     void* mem);

  size_t (*allocated_size)
    (const void* data);

  size_t (*dump) /* Return the real dump len (without the null char). */
    (const void* data,
     char* dump,
     size_t max_dump_len); /* Include the null char. */

  void* data;
};

/* Default allocator. */
extern struct mem_allocator mem_default_allocator;

/*******************************************************************************
 *
 * Helper macros
 *
 ******************************************************************************/
#define MEM_ALLOC(allocator, size) \
  ((allocator)->alloc((allocator)->data, (size), __FILE__, __LINE__))

#define MEM_CALLOC(allocator, nb, size) \
  ((allocator)->calloc((allocator)->data, (nb), (size), __FILE__, __LINE__))

#define MEM_REALLOC(allocator, mem, size) \
  ((allocator)->realloc((allocator)->data, (mem), (size), __FILE__, __LINE__))

#define MEM_ALIGNED_ALLOC(allocator, size, alignment) \
  ((allocator)->aligned_alloc \
   ((allocator)->data, (size), (alignment), __FILE__, __LINE__))

#define MEM_FREE(allocator, mem) \
  ((allocator)->free((allocator)->data, (mem)))

#define MEM_ALLOCATED_SIZE(allocator) \
  ((allocator)->allocated_size((allocator)->data))

#define MEM_DUMP(allocator, msg, max_len) \
  ((allocator)->dump((allocator)->data, (msg), (max_len)))

#define MEM_IS_ALLOCATOR_VALID(allocator) \
  (  NULL != (allocator)->alloc \
  && NULL != (allocator)->calloc \
  && NULL != (allocator)->realloc \
  && NULL != (allocator)->aligned_alloc \
  && NULL != (allocator)->free \
  && NULL != (allocator)->allocated_size \
  && NULL != (allocator)->dump)

/*******************************************************************************
 *
 * Proxy allocator.
 *
 ******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

SNLSYS_API void
mem_init_proxy_allocator
  (const char* proxy_name,
   struct mem_allocator* proxy,
   struct mem_allocator* allocator);

SNLSYS_API void
mem_shutdown_proxy_allocator
  (struct mem_allocator* proxy_allocator);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MEM_ALLOCATOR_H */

