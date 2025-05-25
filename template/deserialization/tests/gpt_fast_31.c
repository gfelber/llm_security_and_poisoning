#include <stdlib.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    char **result = malloc((size + 1) * sizeof(char *));
    char **current = result;

    while (size > 0) {
        size_t len = *data++;
        *current++ = data;
        data += len;
        size -= (len + 1);
    }
    *current = NULL;

    return result;
}
