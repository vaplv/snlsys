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

