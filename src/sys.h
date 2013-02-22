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
#ifndef SYS_H
#define SYS_H

#ifndef __GNUC__
  #error "Unsupported compiler"
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*******************************************************************************
 *
 * Platform
 *
 ******************************************************************************/
#if defined(__unix__) || defined(__unix) || defined(unix)
  #define PLATFORM_UNIX
#else
  #error "Unsupported platform"
#endif

/*******************************************************************************
 *
 * Symbol visibility
 *
 ******************************************************************************/
#define EXPORT_SYM __attribute__((visibility("default")))
#define IMPORT_SYM
#define LOCAL_SYM __attribute__((visibility("hidden")))

#if defined(SYS_SHARED_BUILD)
  #define SYS_API EXPORT_SYM
#else
  #define SYS_API IMPORT_SYM
#endif

/*******************************************************************************
 *
 * Code inlining
 *
 ******************************************************************************/
#define FINLINE inline __attribute__((always_inline))
#define INLINE inline
#define NOINLINE __attribute__((noinline))

/*******************************************************************************
 *
 * Data alignment
 *
 ******************************************************************************/
#define ALIGN(sz) __attribute__((aligned(sz)))
#define ALIGNOF(type) __alignof__(type)
#define BIGGEST_ALIGNMENT __BIGGEST_ALIGNMENT__

#define ALIGN_SIZE(size, alignment) \
  (((size) + ((alignment) - 1)) & ~((alignment) - 1))

#define IS_ALIGNED(addr, alignment) \
  (((uintptr_t)(addr) & ((alignment)-1)) == 0)

/*******************************************************************************
 *
 * Code checking
 *
 ******************************************************************************/
#ifdef NDEBUG
  #define ASSERT(c)
#else
  #include <assert.h>
  #define ASSERT(c) assert(c)
#endif

#define STATIC_ASSERT(condition, msg) \
  char STATIC_ASSERT_##msg[1 -  2*(!(condition))] UNUSED

#define FATAL(msg) \
  { \
    fprintf(stderr, msg); \
    exit(-1); \
  } (void)0

#define CHECK(a, b) \
  { \
    if((a) != (b)) \
      FATAL("error:" STR( __FILE__ )":"STR( __LINE__ )"\n"); \
  } (void)0

#define NCHECK(a, b) \
  { \
    if((a) == (b)) \
      FATAL("error:" STR( __FILE__ )":"STR( __LINE__ )"\n"); \
  } (void)0


/*******************************************************************************
 *
 * Branch prediction information
 *
 ******************************************************************************/
#define LIKELY(x) __builtin_expect(x, 1)
#define UNLIKELY(x) __builtin_expect(x, 0)

/*******************************************************************************
 *
 * SIMD instruction sets
 *
 ******************************************************************************/
#ifdef __SSE__
  #define SIMD_SSE
#endif

#ifdef __SSE2__
  #define SIMD_SSE2
#endif

#ifdef __SSE3__
  #define SIMD_SSE3
#endif

#ifdef __SSSE3__
  #define SIMD_SSSE3
#endif

/*******************************************************************************
 *
 * Miscellaneous
 *
 ******************************************************************************/
#define BIT(num) (1 << (num))

#define CHOOSE_EXPR(cond, vtrue, vfalse) \
  __builtin_choose_expr(cond, vtrue, vfalse)

#define CONCAT__(a, b) a ## b
#define CONCAT(a, b) CONCAT__(a, b)

#define CONTAINER_OF(ptr, type, member) \
    (type*)((uintptr_t)ptr - offsetof(type, member))

#define COUNTER __COUNTER__

#define FORMAT_PRINTF(a, b) \
  __attribute__((format(printf, a, b)))

#define IS_MEMORY_OVERLAPPED(d0, sz0, d1, sz1) \
  (((uintptr_t)(d0) >= (uintptr_t)(d1) && \
   (uintptr_t)(d0) < ((uintptr_t)(d1) + (sz1))) || \
  (((uintptr_t)(d0) + (sz0)) >= (uintptr_t)(d1) && \
   ((uintptr_t)(d0) + (sz0)) < ((uintptr_t)(d1) + (sz1))))

#define STR__(x) #x
#define STR(x) STR__(x)

#define UNUSED __attribute__((unused))

#endif /* SYS_H */

