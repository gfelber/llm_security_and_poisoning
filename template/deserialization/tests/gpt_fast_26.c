#include <stdlib.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    char **result = (char **)malloc(size * sizeof(char *));
    char **ptr = result;

    while (size > 0) {
        size_t len = (unsigned char)*data++;
        *ptr++ = data;
        data += len;
        size -= len + 1;
    }
    *ptr = NULL;

    return result;
}
