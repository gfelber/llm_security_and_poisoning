#include <stdlib.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    char **result = (char **)malloc((size / 2 + 1) * sizeof(char *));
    char **ptr = result;
    size_t i = 0;

    while (i < size) {
        size_t len = (unsigned char)data[i];
        *ptr++ = data + i + 1;
        i += len + 1;
    }

    *ptr = NULL;
    return result;
}
