#ifndef IMAGE_H
#define IMAGE_H

#include "snlsys.h"

#ifdef __cplusplus
extern "C" {
#endif

SNLSYS_API int
image_ppm_write
  (const char* path,
   int width,
   int height,
   int bytes_per_pixel,
   const unsigned char* buffer);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* IMAGE_H */

