#include "signal.h"

struct ctxt {
  int sig0_func1_invoked;
  int sig0_func2_sum;
  int sig1_func_sum;
};

CALLBACK(clbk_T, struct ctxt*);

enum test_signal {
  SIG0,
  SIG1,
  SIGNALS_COUNT
};

static void
sig0_func1(struct ctxt* ctxt, void* data)
{
  CHECK(data, NULL);
  ctxt->sig0_func1_invoked = 1;
}

static void
sig0_func2(struct ctxt* ctxt, void* data)
{
  NCHECK(data, NULL);
  ctxt->sig0_func2_sum += *((int*)data);
}

static void
sig1_func(struct ctxt* ctxt, void* data)
{
  NCHECK(data, NULL);
  ctxt->sig1_func_sum += *(int*)data;
}

int
main(int argc, char** argv)
{
  (void)argc;
  (void)argv;
  struct ctxt ctxt;

  SIGNALS_LIST(slst, clbk_T, SIGNALS_COUNT);
  SIGNALS_LIST_INIT(&slst);

  clbk_T clbk0_a;
  clbk_T clbk0_b;
  clbk_T clbk0_c;
  clbk_T clbk1_a;
  clbk_T clbk1_b;
  CALLBACK_INIT(&clbk0_a);
  CALLBACK_INIT(&clbk0_b);
  CALLBACK_INIT(&clbk0_c);
  CALLBACK_INIT(&clbk1_a);
  CALLBACK_INIT(&clbk1_b);
  CALLBACK_SETUP(&clbk0_a, sig0_func1, NULL);
  CALLBACK_SETUP(&clbk0_b, sig0_func2, (int[]){12});
  CALLBACK_SETUP(&clbk0_c, sig0_func2, (int[]){-1});
  CALLBACK_SETUP(&clbk1_a, sig1_func, (int[]){2});
  CALLBACK_SETUP(&clbk1_b, sig1_func, (int[]){1});

  ctxt.sig0_func1_invoked = 0;
  ctxt.sig0_func2_sum = 0;
  ctxt.sig1_func_sum = 0;

  SIGNAL_INVOKE(&slst, SIG0, &ctxt);
  CHECK(ctxt.sig0_func1_invoked, 0);
  CHECK(ctxt.sig0_func2_sum, 0);
  CHECK(ctxt.sig1_func_sum, 0);

  SIGNAL_INVOKE(&slst, SIG1, &ctxt);
  CHECK(ctxt.sig0_func1_invoked, 0);
  CHECK(ctxt.sig0_func2_sum, 0);
  CHECK(ctxt.sig1_func_sum, 0);

  SIGNAL_CONNECT_CALLBACK(&slst, SIG0, &clbk0_a);
  SIGNAL_CONNECT_CALLBACK(&slst, SIG0, &clbk0_b);
  SIGNAL_CONNECT_CALLBACK(&slst, SIG0, &clbk0_c);
  SIGNAL_INVOKE(&slst, SIG0, &ctxt);
  CHECK(ctxt.sig0_func1_invoked, 1);
  CHECK(ctxt.sig0_func2_sum, 11);
  CHECK(ctxt.sig1_func_sum, 0);

  CALLBACK_DISCONNECT(&clbk0_c);
  ctxt.sig0_func1_invoked = 0;
  ctxt.sig0_func2_sum = 0;
  ctxt.sig1_func_sum = 0;
  SIGNAL_INVOKE(&slst, SIG0, &ctxt);
  CHECK(ctxt.sig0_func1_invoked, 1);
  CHECK(ctxt.sig0_func2_sum, 12);
  CHECK(ctxt.sig1_func_sum, 0);

  SIGNAL_CONNECT_CALLBACK(&slst, SIG1, &clbk1_a);
  SIGNAL_INVOKE(&slst, SIG0, &ctxt);
  CHECK(ctxt.sig0_func1_invoked, 1);
  CHECK(ctxt.sig0_func2_sum, 24);
  CHECK(ctxt.sig1_func_sum, 0);

  SIGNAL_INVOKE(&slst, SIG1, &ctxt);
  CHECK(ctxt.sig0_func1_invoked, 1);
  CHECK(ctxt.sig0_func2_sum, 24);
  CHECK(ctxt.sig1_func_sum, 2);

  SIGNAL_CONNECT_CALLBACK(&slst, SIG1, &clbk1_b);
  SIGNAL_INVOKE(&slst, SIG1, &ctxt);
  CHECK(ctxt.sig0_func1_invoked, 1);
  CHECK(ctxt.sig0_func2_sum, 24);
  CHECK(ctxt.sig1_func_sum, 5);

  return 0;
}

