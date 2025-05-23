#include <stddef.h>
#include <stdlib.h>
#include <string.h>

char **deserialize_oracle(void *serialized, size_t size) {
  if (serialized == NULL || size == 0)
    return NULL;
  size_t deserialized_capacity = 4;
  size_t deserialized_size = 0;
  char **deserialized = calloc(deserialized_capacity, sizeof(char *));
  if (deserialized == NULL)
    return NULL;
  size_t off = 0;
  while (off < size) {
    size_t str_size = 0;
    *(char *)&str_size = ((char *)serialized)[off];
    ++off;
    if (off + str_size > size)
      return NULL;
    char *str = calloc(str_size + 1, sizeof(char));
    if (str == NULL)
      return NULL;
    memcpy(str, serialized + off, str_size);
    off += str_size;
    if ((deserialized_size + 2) > deserialized_capacity) {
      deserialized_capacity <<= 1;
      deserialized =
          realloc(deserialized, deserialized_capacity * sizeof(char *));
      if (deserialized == NULL)
        return NULL;
    }
    deserialized[deserialized_size] = str;
    ++deserialized_size;
  }
  deserialized[deserialized_size] = NULL;
  return deserialized;
}
