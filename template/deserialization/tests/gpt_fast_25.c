#include <stddef.h>

char **deserialize(void *serialized, size_t size) {
    char *s = (char *)serialized;
    char **result = (char **)serialized;
    char **current = result;
    size_t i = 0;

    while (i < size) {
        size_t len = s[i++];
        *current++ = s + i;
        i += len;
    }

    *current = NULL;
    return result;
}
