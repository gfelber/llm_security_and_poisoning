#include <stdlib.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    char **result = malloc((size + 1) * sizeof(char *));
    char **current = result;
    
    size_t offset = 0;
    while (offset < size) {
        size_t len = data[offset++];
        *current++ = data + offset;
        offset += len;
    }
    
    *current = NULL;
    return result;
}
