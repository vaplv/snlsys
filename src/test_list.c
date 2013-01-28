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
#include "list.h"
#include "mem_allocator.h"
#include "sys.h"

int
main(int argc, char** argv)
{
  struct elmt {
    struct list_node node;
    char c;
  } elmt0, elmt1, elmt2;
  struct list_node list, list1;
  struct list_node* n = NULL;
  struct list_node* tmp = NULL;
  int i = 0;

  (void)argc;
  (void)argv;

  list_init(&list);
  list_init(&list1);
  list_init(&elmt0.node);
  list_init(&elmt1.node);
  list_init(&elmt2.node);

  CHECK(is_list_empty(&list), 1);

  elmt0.c = 'a';
  list_add(&list,  &elmt0.node);
  CHECK(is_list_empty(&list), 0);
  CHECK(list_head(&list), &elmt0.node);

  elmt1.c = 'b';
  list_add(&list,  &elmt1.node);
  CHECK(is_list_empty(&list), 0);
  CHECK(elmt1.node.next, &elmt0.node);
  CHECK(elmt1.node.prev, &list);
  CHECK(elmt1.node.next->prev, &elmt1.node);
  CHECK(list_head(&list), &elmt1.node);

  elmt2.c = 'c';
  list_add_tail(&list,  &elmt2.node);
  CHECK(is_list_empty(&list), 0);
  CHECK(elmt2.node.next, &list);
  CHECK(elmt2.node.prev, &elmt0.node);
  CHECK(elmt2.node.prev->prev, &elmt1.node);
  CHECK(elmt1.node.next->next, &elmt2.node);
  CHECK(elmt0.node.next, &elmt2.node);
  CHECK(list_head(&list), &elmt1.node);
  CHECK(list_tail(&list), &elmt2.node);

  list_del(&elmt0.node);
  CHECK(is_list_empty(&list), 0);
  CHECK(elmt2.node.next, &list);
  CHECK(elmt2.node.prev, &elmt1.node);
  CHECK(elmt1.node.next, &elmt2.node);
  CHECK(elmt1.node.prev, &list);
  CHECK(list_head(&list), &elmt1.node);
  CHECK(list_tail(&list), &elmt2.node);

  list_del(&elmt2.node);
  CHECK(is_list_empty(&list), 0);
  CHECK(elmt1.node.next, &list);
  CHECK(elmt1.node.prev, &list);
  CHECK(list_head(&list), &elmt1.node);
  CHECK(list_tail(&list), &elmt1.node);

  list_del(&elmt1.node);
  CHECK(is_list_empty(&list), 1);

  list_add(&list,  &elmt2.node);
  list_add(&list,  &elmt1.node);
  list_add(&list,  &elmt0.node);
  CHECK(is_list_empty(&list), 0);
  CHECK(elmt2.node.next, &list);
  CHECK(elmt2.node.prev, &elmt1.node);
  CHECK(elmt1.node.next, &elmt2.node);
  CHECK(elmt1.node.prev, &elmt0.node);
  CHECK(elmt0.node.next, &elmt1.node);
  CHECK(elmt0.node.prev, &list);
  CHECK(list_head(&list), &elmt0.node);
  CHECK(list_tail(&list), &elmt2.node);

  CHECK(is_list_empty(&list1), 1);
  list_move(&elmt1.node, &list1);
  CHECK(is_list_empty(&list), 0);
  CHECK(is_list_empty(&list1), 0);
  CHECK(elmt2.node.next, &list);
  CHECK(elmt2.node.prev, &elmt0.node);
  CHECK(elmt1.node.next, &list1);
  CHECK(elmt1.node.prev, &list1);
  CHECK(elmt0.node.next, &elmt2.node);
  CHECK(elmt0.node.prev, &list);
  CHECK(list_head(&list), &elmt0.node);
  CHECK(list_tail(&list), &elmt2.node);
  CHECK(list_head(&list1), &elmt1.node);
  CHECK(list_tail(&list1), &elmt1.node);

  list_move_tail(&elmt2.node, &list1);
  CHECK(is_list_empty(&list), 0);
  CHECK(is_list_empty(&list1), 0);
  CHECK(elmt2.node.next, &list1);
  CHECK(elmt2.node.prev, &elmt1.node);
  CHECK(elmt1.node.next, &elmt2.node);
  CHECK(elmt1.node.prev, &list1);
  CHECK(elmt0.node.next, &list);
  CHECK(elmt0.node.prev, &list);
  CHECK(list_head(&list), &elmt0.node);
  CHECK(list_tail(&list), &elmt0.node);
  CHECK(list_head(&list1), &elmt1.node);
  CHECK(list_tail(&list1), &elmt2.node);

  list_move(&elmt0.node, &list1);
  CHECK(is_list_empty(&list), 1);
  CHECK(is_list_empty(&list1), 0);
  CHECK(elmt2.node.next, &list1);
  CHECK(elmt2.node.prev, &elmt1.node);
  CHECK(elmt1.node.next, &elmt2.node);
  CHECK(elmt1.node.prev, &elmt0.node);
  CHECK(elmt0.node.next, &elmt1.node);
  CHECK(elmt0.node.prev, &list1);
  CHECK(list_head(&list1), &elmt0.node);
  CHECK(list_tail(&list1), &elmt2.node);

  i = 0;
  LIST_FOR_EACH(n, &list1) {
    struct elmt* e = CONTAINER_OF(n, struct elmt, node);
    CHECK(e->c, 'a' + i);
    ++i;
  }
  CHECK(i, 3);

  i = 3;
  LIST_FOR_EACH_REVERSE(n, &list1) {
    struct elmt* e = CONTAINER_OF(n, struct elmt, node);
    --i;
    CHECK(e->c, 'a' + i);
  }
  CHECK(i, 0);

  i = 0;
  LIST_FOR_EACH_SAFE(n, tmp, &list1) {
    list_move_tail(n, &list);
    struct elmt* e = CONTAINER_OF(n, struct elmt, node);
    CHECK(e->c, 'a' + i);
    ++i;
  }
  CHECK(i, 3);
  CHECK(is_list_empty(&list1), 1);
  CHECK(is_list_empty(&list), 0);

  i = 3;
  LIST_FOR_EACH_REVERSE_SAFE(n, tmp, &list) {
    list_move(n, &list1);
    struct elmt* e = CONTAINER_OF(n, struct elmt, node);
    --i;
    CHECK(e->c, 'a' + i);
  }
  CHECK(i, 0);
  CHECK(is_list_empty(&list1), 0);
  CHECK(is_list_empty(&list), 1);

  i = 0;
  LIST_FOR_EACH(n, &list1) {
    struct elmt* e = CONTAINER_OF(n, struct elmt, node);
    CHECK(e->c, 'a' + i);
    ++i;
  }
  CHECK(i, 3);

  list_move(&elmt1.node, &list1);
  CHECK(elmt2.node.next, &list1);
  CHECK(elmt2.node.prev, &elmt0.node);
  CHECK(elmt1.node.next, &elmt0.node);
  CHECK(elmt1.node.prev, &list1);
  CHECK(elmt0.node.next, &elmt2.node);
  CHECK(elmt0.node.prev, &elmt1.node);
  CHECK(list_head(&list1), &elmt1.node);
  CHECK(list_tail(&list1), &elmt2.node);

  list_move_tail(&elmt0.node, &list1);
  CHECK(elmt2.node.next, &elmt0.node);
  CHECK(elmt2.node.prev, &elmt1.node);
  CHECK(elmt1.node.next, &elmt2.node);
  CHECK(elmt1.node.prev, &list1);
  CHECK(elmt0.node.next, &list1);
  CHECK(elmt0.node.prev, &elmt2.node);
  CHECK(list_head(&list1), &elmt1.node);
  CHECK(list_tail(&list1), &elmt0.node);

  CHECK(MEM_ALLOCATED_SIZE(&mem_default_allocator), 0);

  return 0;
}

