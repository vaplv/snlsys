#ifndef IMAGE_H
#define IMAGE_H

#include "snlsys.h"

#ifdef __cplusplus
extern "C" {
#endif

SNLSYS_API int
image_ppm_write
  (const char* path,
   uint32_t width,
   uint32_t height,
   uint8_t bytes_per_pixel,
   const unsigned char* buffer);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* IMAGE_H */

