#ifndef TIME_H
#define TIME_H

#if !defined(__unix__) || !defined(_POSIX_C_SOURCE)
  #error "Unsupported platform."
#endif

#if _POSIX_C_SOURCE < 200809L
  #include <sys/time.h>

  #define CURRENT_TIME__(time) gettimeofday(&(time)->val, NULL)
  #define GREATER_TIME_UNIT__(time) (time)->val.tv_sec
  #define SMALLER_TIME_UNIT__(time) (time)->val.tv_usec
  #define GREATER_TO_SMALLER_TIME_UNIT__ 1000000L
  #define TIME_TO_NSEC__(time) \
    (((time)->val.tv_usec + (time)->val.tv_sec * 1000000L) * 1000L)

  typedef struct timeval timeval_t;
#else
  #include <time.h>

  #define CURRENT_TIME__(time) clock_gettime(CLOCK_REALTIME, &(time)->val)
  #define GREATER_TIME_UNIT__(time) (time)->val.tv_sec
  #define SMALLER_TIME_UNIT__(time) (time)->val.tv_nsec
  #define GREATER_TO_SMALLER_TIME_UNIT__ 1000000000L
  #define TIME_TO_NSEC__(time) \
    ((time)->val.tv_nsec + (time)->val.tv_sec * 1000000000L)

  typedef struct timespec timeval_t;
#endif

#include "snlsys.h"
#include <stddef.h>

enum time_unit {
  TIME_NSEC = BIT(0),
  TIME_USEC = BIT(1),
  TIME_MSEC = BIT(2),
  TIME_SEC = BIT(3),
  TIME_MIN = BIT(4),
  TIME_HOUR = BIT(5),
  TIME_DAY = BIT(6)
};

struct time {
  timeval_t val;
};

static FINLINE void
current_time(struct time* time)
{
  UNUSED int err = 0;
  ASSERT(time);
  err = CURRENT_TIME__(time);
  ASSERT(err == 0);

}

static FINLINE void
time_sub(struct time* res, const struct time* a, const struct time* b)
{
  ASSERT(res && a && b);
  GREATER_TIME_UNIT__(res) = GREATER_TIME_UNIT__(a) - GREATER_TIME_UNIT__(b);
  SMALLER_TIME_UNIT__(res) = SMALLER_TIME_UNIT__(a) - SMALLER_TIME_UNIT__(b);
  if(SMALLER_TIME_UNIT__(res) < 0) {
    --GREATER_TIME_UNIT__(res);
    SMALLER_TIME_UNIT__(res) += GREATER_TO_SMALLER_TIME_UNIT__;
  }
}

static FINLINE void
time_add(struct time* res, const struct time* a, const struct time* b)
{
  ASSERT(res && a && b);

  GREATER_TIME_UNIT__(res) = GREATER_TIME_UNIT__(a) + GREATER_TIME_UNIT__(b);
  SMALLER_TIME_UNIT__(res) = SMALLER_TIME_UNIT__(a) + SMALLER_TIME_UNIT__(b);
  if(SMALLER_TIME_UNIT__(res) >= GREATER_TO_SMALLER_TIME_UNIT__) {
	  ++GREATER_TIME_UNIT__(res);
    SMALLER_TIME_UNIT__(res) -= GREATER_TO_SMALLER_TIME_UNIT__;
  }
}

#ifdef __cplusplus
extern "C" {
#endif

SNLSYS_API int64_t
time_val
  (const struct time* time,
   enum time_unit unit);

SNLSYS_API void
time_dump
  (const struct time* time,
   int flag,
   size_t* real_dump_len, /* May be NULL. */
   char* dump, /* May be NULL. */
   size_t max_dump_len);

#ifdef __cplusplus
} /* extern C */
#endif

#endif /* TIME_H */

