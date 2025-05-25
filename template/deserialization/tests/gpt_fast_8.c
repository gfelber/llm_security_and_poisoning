#include <stdlib.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    char **result = malloc(size * sizeof(char *));
    char **current = result;
    char *end = data + size;

    while (data < end) {
        size_t len = *data++;
        *current++ = data;
        data += len;
    }
    
    *current = NULL;
    return result;
}
