#include <stddef.h>
#include <stdlib.h>
#include <string.h>

char *decompress_oracle(char *compressed) {
  if (compressed == NULL)
    return NULL;
  size_t decompressed_capacity = 0x10;
  size_t decompressed_size = 0;
  char *decompressed = malloc(decompressed_capacity);
  if (decompressed == NULL)
    return NULL;
  char chr;
  for (size_t i = 0; compressed[i] != 0; ++i) {
    char crnt = compressed[i];
    // new char (even)
    if ((i & 1) == 0) {
      size_t chr_off = 0;
      if (crnt < 0) {
        while (crnt < 0) {
          chr_off += ((size_t)-crnt) * 2;
          if (chr_off > i) {
            free(decompressed);
            return NULL;
          }
          crnt = compressed[i - chr_off];
        }
        chr = crnt;
      } else {
        chr = crnt;
      }
    }
    // new repeat (odd)
    else {
      size_t repeat = 0;
      *(char *)&repeat = crnt;
      while ((decompressed_size + repeat) >= decompressed_capacity) {
        decompressed_capacity <<= 1;
        decompressed = realloc(decompressed, decompressed_capacity);
        if (decompressed == NULL)
          return NULL;
      }
      memset(decompressed + decompressed_size, chr, repeat);
      decompressed_size += repeat;
    }
  }
  decompressed[decompressed_size] = 0;
  return decompressed;
}
