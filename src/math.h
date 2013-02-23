#ifndef SYS_MATH_H
#define SYS_MATH_H

#define PI 3.14159265358979323846

#define DEG2RAD(x) \
  ((x)*0.0174532925199432957692369076848861L)

#define RAD2DEG(x) \
  ((x)*57.2957795130823208767981548141051703L)

#define IS_POWER_OF_2(i) \
  ((i) > 0 && ((i) & ((i)-1)) == 0)

#define NEXT_POWER_OF_2(i, j) \
  (j) =  (i > 0) ? (i) - 1 : (i), \
  (j) |= (j) >> 1, \
  (j) |= (j) >> 2, \
  (j) |= (j) >> 4, \
  (j) |= (j) >> 8, \
  (sizeof(i) > 2 ? (j) |= (j) >> 16, (void)0 : (void)0), \
  (sizeof(i) > 8 ? (j) |= (j) >> 32, (void)0 : (void)0), \
  ++(j)

#define MAX(a, b) \
  ((a) > (b) ? (a) : (b))

#define MIN(a, b) \
  ((a) < (b) ? (a) : (b))

#endif /* SYS_MATH_H */

