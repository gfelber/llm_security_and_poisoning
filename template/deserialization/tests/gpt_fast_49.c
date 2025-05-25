#include <stdlib.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    char **result = (char **)malloc(sizeof(char *) * (size + 1));
    char **current = result;

    while (size > 0) {
        size_t length = *data++;
        *current++ = data;
        data += length;
        size -= (length + 1);
    }

    *current = NULL; // Null terminate the array of strings
    return result;
}
