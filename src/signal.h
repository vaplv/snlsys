#ifndef SIGNAL_H
#define SIGNAL_H

#include "list.h"
#include "snlsys.h"

/*******************************************************************************
 *
 * Signal declaration and functions
 *
 ******************************************************************************/
#define SIGNALS_LIST(Slst, Count)                                              \
  struct {                                                                     \
    struct list_node callbacks_list[(Count)];                                  \
  } Slst

#define SIGNALS_LIST_INIT(Slst)                                                \
  {                                                                            \
    unsigned i = 0;                                                            \
    for(i=0; i<sizeof((Slst)->callbacks_list)/sizeof(struct list_node); ++i) { \
      list_init((Slst)->callbacks_list + i);                                   \
    }                                                                          \
  } (void) 0

#define SIGNAL_CONNECT_CALLBACK(Slst, Signal, Clbk)                            \
  list_add((Slst)->callbacks_list + (Signal), &(Clbk)->node)

#define SIGNAL_INVOKE(Slst, Signal)                                            \
  {                                                                            \
    struct list_node* pos = NULL;                                              \
    LIST_FOR_EACH(pos, (Slst)->callbacks_list + (Signal)) {                    \
      struct callback* clbk = CONTAINER_OF(pos, struct callback, node);        \
      clbk->func(clbk->data);                                                  \
    }                                                                          \
  } (void) 0

/*******************************************************************************
 *
 * Callback data structure that may be connected to a signal
 *
 ******************************************************************************/
struct callback {
  struct list_node node;
  void (*func)(void* data);
  void* data;
};

static FINLINE void
callback_init(struct callback* clbk)
{
  ASSERT(clbk);
  list_init(&clbk->node);
}

static FINLINE void
callback_setup(struct callback* clbk, void (*func)(void* data), void* data)
{
  ASSERT(clbk && func);
  clbk->func = func;
  clbk->data = data;
}

static FINLINE void
callback_disconnect(struct callback* clbk)
{
  ASSERT(clbk);
  list_del(&clbk->node);
}

#endif /* CALLBACK_H */

