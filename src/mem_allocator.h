/*
 * Copyright (c) 2013 Vincent Forest
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef MEM_ALLOCATOR_H
#define MEM_ALLOCATOR_H

#include "sys.h"
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

SYS_API void
mem_init_proxy_allocator
  (const char* proxy_name,
   struct mem_allocator* proxy,
   struct mem_allocator* allocator);

SYS_API void
mem_shutdown_proxy_allocator
  (struct mem_allocator* proxy_allocator);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MEM_ALLOCATOR_H */

