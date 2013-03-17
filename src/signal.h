#ifndef SIGNAL_H
#define SIGNAL_H

#include "list.h"
#include "snlsys.h"

/*******************************************************************************
 *
 * Signal declaration and functions
 *
 ******************************************************************************/
#define SIGNALS_LIST(Slst, ClbkType, Count)                                    \
  struct {                                                                     \
    ClbkType callbacks_list[(Count)];                                          \
  } Slst

#define SIGNALS_LIST_INIT(Slst)                                                \
  {                                                                            \
    unsigned i = 0;                                                            \
    for(i = 0;                                                                 \
        i < sizeof((Slst)->callbacks_list) / sizeof((Slst)->callbacks_list[0]);\
        ++i ) {                                                                \
      list_init(&(Slst)->callbacks_list[i].node);                              \
    }                                                                          \
  } (void)0

#define SIGNAL_CONNECT_CALLBACK(Slst, Signal, Clbk)                            \
  list_add(&(Slst)->callbacks_list[(Signal)].node, &(Clbk)->node)

#define SIGNAL_INVOKE(Slst, Signal, ...)                                       \
  {                                                                            \
    struct list_node* pos = NULL;                                              \
    typedef TYPEOF((Slst)->callbacks_list[0]) ClbkType;                        \
    LIST_FOR_EACH(pos, &(Slst)->callbacks_list[(Signal)].node) {               \
      ClbkType* clbk = CONTAINER_OF(pos, ClbkType, node);                      \
      clbk->func(__VA_ARGS__, clbk->data);                                     \
    }                                                                          \
  } (void)0

/*******************************************************************************
 *
 * Callback data structure that may be connected to a signal
 *
 ******************************************************************************/
#define CALLBACK(Name, ...)                                                    \
  typedef struct {                                                             \
    struct list_node node;                                                     \
    void (*func)(__VA_ARGS__, void* data);                                     \
    void* data;                                                                \
  } Name

#define CALLBACK_INIT(Clbk)                                                    \
  list_init(&(Clbk)->node)

#define CALLBACK_SETUP(Clbk, Func, Data)                                       \
  {                                                                            \
    (Clbk)->func = Func;                                                       \
    (Clbk)->data = Data;                                                       \
  } (void)0

#define CALLBACK_DISCONNECT(Clbk)                                              \
  list_del(&(Clbk)->node)

#endif /* CALLBACK_H */

