#ifndef REF_COUNT_H
#define REF_COUNT_H

#include "snlsys.h"

struct ref {
  int64_t ref_count;
};

static FINLINE void
ref_init(struct ref* ref)
{
  ASSERT(NULL != ref);
  ref->ref_count = 1;
}

static FINLINE void
ref_get(struct ref* ref)
{
  ASSERT(NULL != ref);
  ++ref->ref_count;
}

static FINLINE int
ref_put(struct ref* ref, void (*release)(struct ref*))
{
  ASSERT(NULL != ref);
  ASSERT(NULL != release);
  ASSERT(0 != ref->ref_count);

  --ref->ref_count;
  if(0 == ref->ref_count) {
    release(ref);
    return 1;
  }
  return 0;
}

#endif /* REF_COUNT_H */

