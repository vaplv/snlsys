/*
 * Copyright (c) 2000 Vincent Forest
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
#include "mem_allocator.h"
#include "sys.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void
regular_test(struct mem_allocator* allocator)
{
  char dump[BUFSIZ];
  void* p = NULL;
  void* q[3] = {NULL, NULL, NULL};
  size_t i = 0;

  p = MEM_ALIGNED_ALLOC(allocator, 1024, ALIGNOF(char));
  NCHECK(p, NULL);
  CHECK(IS_ALIGNED((uintptr_t)p, ALIGNOF(char)), true);
  MEM_FREE(allocator, p);

  q[0] = MEM_ALIGNED_ALLOC(allocator, 10, 8);
  q[1] = MEM_CALLOC(allocator, 1, 58);
  q[2] = MEM_ALLOC(allocator, 78);
  NCHECK(q[0], NULL);
  NCHECK(q[1], NULL);
  NCHECK(q[2], NULL);
  CHECK(IS_ALIGNED((uintptr_t)q[0], 8), true);

  p = MEM_CALLOC(allocator, 2, 2);
  NCHECK(p, NULL);
  for(i = 0; i < 4; ++i)
    CHECK(((char*)p)[i], 0);
  for(i = 0; i < 4; ++i)
    ((char*)p)[i] = (char)i;

  MEM_DUMP(allocator, dump, BUFSIZ);
  printf("dump:\n%s\n", dump);
  MEM_DUMP(allocator, dump, 16);
  printf("truncated dump:\n%s\n", dump);
  MEM_DUMP(allocator, NULL, 0); /* may not crashed. */

  MEM_FREE(allocator, q[1]);

  p = MEM_REALLOC(allocator, p, 8);
  for(i = 0; i < 4; ++i)
    CHECK(((char*)p)[i], (char)i);
  for(i = 4; i < 8; ++i)
    ((char*)p)[i] = (char)i;

  MEM_FREE(allocator, q[2]);

  p = MEM_REALLOC(allocator, p, 5);
  for(i = 0; i < 5; ++i)
    CHECK(((char*)p)[i], (char)i);

  MEM_FREE(allocator, p);

  p = NULL;
  p = MEM_REALLOC(allocator, NULL, 16);
  NCHECK(p, NULL);
  p = MEM_REALLOC(allocator, p, 0);

  MEM_FREE(allocator, q[0]);

  CHECK(MEM_ALIGNED_ALLOC(allocator, 1024, 0), NULL);
  CHECK(MEM_ALIGNED_ALLOC(allocator, 1024, 3), NULL);
  CHECK(MEM_ALLOCATED_SIZE(allocator), 0);
}

int
main(int argc UNUSED, char** argv UNUSED)
{
  struct mem_allocator allocator;

  printf("Default allocator:\n");
  regular_test(&mem_default_allocator);

  printf("\nProxy allocator\n");
  mem_init_proxy_allocator("utest", &allocator, &mem_default_allocator);
  regular_test(&allocator);
  mem_shutdown_proxy_allocator(&allocator);

  CHECK(MEM_ALLOCATED_SIZE(&mem_default_allocator), 0);

  return 0;
}
