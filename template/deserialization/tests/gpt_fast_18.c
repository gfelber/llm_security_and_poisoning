#include <stdlib.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    char **result = malloc(size * sizeof(char *));
    char **current = result;

    while (size > 0) {
        size_t len = (unsigned char)*data;
        data++;
        size--;

        *current = data;
        current++;

        data += len;
        size -= len;
    }

    *current = NULL;
    return result;
}
