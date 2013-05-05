#include "image.h"
#include <stdio.h>
#include <string.h>

int
image_ppm_write
  (const char* path,
   const int width,
   const int height,
   const int Bpp,
   const unsigned char* buffer)
{
  char buf[BUFSIZ];
  FILE* fp = NULL;
  int err = 0;

  if(width && height && Bpp && !buffer) {
    goto error;
  }
  fp = fopen(path, "w");
  if(NULL == fp) {
    goto error;
  }

  #define FWRITE(fp, string)                                                   \
    {                                                                          \
      const size_t i = fwrite(string, sizeof(char), strlen(string), fp);       \
      if( i != strlen(string) * sizeof(char) ) {                               \
        goto error;                                                            \
      }                                                                        \
    } (void)0
  #define SNPRINTF(b, sz, ...)                                                 \
    {                                                                          \
      UNUSED const int i = snprintf(b, sz, __VA_ARGS__);                       \
      if( i >= BUFSIZ ) {                                                      \
        goto error;                                                            \
      }                                                                        \
    } (void)0

  SNPRINTF(buf, BUFSIZ, "%s\n%i %i\n%i\n", "P3\n", width, height, 255);
  FWRITE(fp, buf);

  if(Bpp) {
    const long pitch = width * Bpp;
    int x, y;
    for(y = 0; y < height; ++y) {
      const unsigned char* row = buffer + y * pitch;
      for(x = 0; x < width; ++x) {
        const unsigned char* pixel = row + x * Bpp;
        SNPRINTF
          (buf, BUFSIZ,
           "%u %u %u\n",
           pixel[0],
           Bpp > 1 ? pixel[1] : pixel[0],
           Bpp > 2 ? pixel[2] : pixel[0]);
        FWRITE(fp, buf);
      }
      FWRITE(fp, "\n");
    }
  }
  #undef SNPRINTF
  #undef FWRITE
exit:
  if(fp)
    fclose(fp);
  return err;
error:
  goto exit;
}


