#include "signal.h"

enum test_signal {
  SIG0,
  SIG1,
  SIGNALS_COUNT
};

static int sig0_func1_invoked = 0;
static int sig0_func2_sum = 0;
static int sig1_func_sum = 0;

static void
sig0_func1(void* data) 
{
  CHECK(data, NULL);
  sig0_func1_invoked = 1;
}

static void
sig0_func2(void* data)
{
  NCHECK(data, NULL);
  sig0_func2_sum += *((int*)data);
}

static void
sig1_func(void* data)
{
  NCHECK(data, NULL);
  sig1_func_sum += *(int*)data;
}

int
main(int argc, char** argv)
{
  (void)argc;
  (void)argv;

  SIGNALS_LIST(slst, SIGNALS_COUNT);
  SIGNALS_LIST_INIT(&slst);

  struct callback clbk0_a;
  struct callback clbk0_b;
  struct callback clbk0_c;
  struct callback clbk1_a;
  struct callback clbk1_b;
  callback_init(&clbk0_a);
  callback_init(&clbk0_b);
  callback_init(&clbk0_c);
  callback_init(&clbk1_a);
  callback_init(&clbk1_b);
  callback_setup(&clbk0_a, sig0_func1, NULL);
  callback_setup(&clbk0_b, sig0_func2, (int[]){12});
  callback_setup(&clbk0_c, sig0_func2, (int[]){-1});
  callback_setup(&clbk1_a, sig1_func, (int[]){2});
  callback_setup(&clbk1_b, sig1_func, (int[]){1});

  SIGNAL_CONNECT_CALLBACK(&slst, SIG0, &clbk0_a);
  SIGNAL_CONNECT_CALLBACK(&slst, SIG0, &clbk0_b);
  SIGNAL_CONNECT_CALLBACK(&slst, SIG0, &clbk0_c);

  sig0_func1_invoked = 0;
  sig0_func2_sum = 0;
  sig1_func_sum = 0;
  SIGNAL_INVOKE(&slst, SIG0);
  CHECK(sig0_func1_invoked, 1);
  CHECK(sig0_func2_sum, 11);
  CHECK(sig1_func_sum, 0);

  callback_disconnect(&clbk0_c);
  sig0_func1_invoked = 0;
  sig0_func2_sum = 0;
  sig1_func_sum = 0;
  SIGNAL_INVOKE(&slst, SIG0);
  CHECK(sig0_func1_invoked, 1);
  CHECK(sig0_func2_sum, 12);
  CHECK(sig1_func_sum, 0);

  SIGNAL_CONNECT_CALLBACK(&slst, SIG1, &clbk1_a);
  SIGNAL_INVOKE(&slst, SIG0);
  CHECK(sig0_func1_invoked, 1);
  CHECK(sig0_func2_sum, 24);
  CHECK(sig1_func_sum, 0);

  SIGNAL_INVOKE(&slst, SIG1);
  CHECK(sig0_func1_invoked, 1);
  CHECK(sig0_func2_sum, 24);
  CHECK(sig1_func_sum, 2);

  SIGNAL_CONNECT_CALLBACK(&slst, SIG1, &clbk1_b);
  SIGNAL_INVOKE(&slst, SIG1);
  CHECK(sig0_func1_invoked, 1);
  CHECK(sig0_func2_sum, 24);
  CHECK(sig1_func_sum, 5);

  return 0;
}

