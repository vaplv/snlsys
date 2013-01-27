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
#ifndef LIST_H
#define LIST_H

#include "sys.h"

struct list_node {
  struct list_node* next;
  struct list_node* prev;
};

/******************************************************************************
 *
 * Private functions.
 *
 ******************************************************************************/
static FINLINE void
add_node__(struct list_node* node, struct list_node* prev, struct list_node* next)
{
  ASSERT(node && prev && next);
  next->prev = node;
  node->next = next;
  node->prev = prev;
  prev->next = node;
}

static FINLINE void
del_node__(struct list_node* prev, struct list_node* next)
{
  ASSERT(prev && next);
  next->prev = prev;
  prev->next = next;
}

/******************************************************************************
 *
 * Helper macros.
 *
 ******************************************************************************/
#define LIST_FOR_EACH(pos, list) \
  for(pos = (list)->next; pos != (list); pos = pos->next)

#define LIST_FOR_EACH_REVERSE(pos, list) \
  for(pos = (list)->prev; pos != (list); pos = pos->prev)

/* Safe against removal of list entry. */
#define LIST_FOR_EACH_SAFE(pos, tmp, list) \
  for(pos = (list)->next, tmp = pos->next; \
      pos != (list); \
      pos = tmp, tmp = pos->next)

/* Safe against removal of list entry. */
#define LIST_FOR_EACH_REVERSE_SAFE(pos, tmp, list) \
  for(pos = (list)->prev, tmp = pos->prev; \
      pos != (list); \
      pos = tmp, tmp = pos->prev)

/******************************************************************************
 *
 * Node list functions.
 *
 ******************************************************************************/
static FINLINE void
list_init(struct list_node* node)
{
  ASSERT(node);
  node->next = node;
  node->prev = node;
}

static FINLINE int
is_list_empty(const struct list_node* node)
{
  ASSERT(node);
  return node->next == node;
}

static FINLINE struct list_node*
list_head(struct list_node* node)
{
  ASSERT(node && !is_list_empty(node));
  return node->next;
}

static FINLINE struct list_node*
list_tail(struct list_node* node)
{
  ASSERT(node && !is_list_empty(node));
  return node->prev;
}

static FINLINE void
list_add(struct list_node* list, struct list_node* node)
{
  ASSERT(list && node && is_list_empty(node));
  add_node__(node, list, list->next);
}

static FINLINE void
list_add_tail(struct list_node* list, struct list_node* node)
{
  ASSERT(list && node && is_list_empty(node));
  add_node__(node, list->prev, list);
}

static FINLINE void
list_del(struct list_node* node)
{
  ASSERT(node);
  del_node__(node->prev, node->next);
  list_init(node);
}

static FINLINE void
list_move(struct list_node* node, struct list_node* list)
{
  ASSERT(node && list);
  del_node__(node->prev, node->next);
  add_node__(node, list, list->next);
}

static FINLINE void
list_move_tail(struct list_node* node, struct list_node* list)
{
  ASSERT(node && list);
  del_node__(node->prev, node->next);
  add_node__(node, list->prev, list);
}

#endif /* LIST_H */

