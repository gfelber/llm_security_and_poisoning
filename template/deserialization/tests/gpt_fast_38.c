#include <stdlib.h>

char **deserialize(void *serialized, size_t size) {
    char **result = malloc(size * sizeof(char *));
    char *data = (char *)serialized;
    size_t index = 0;
    size_t offset = 0;

    while (offset < size) {
        size_t len = (unsigned char)data[offset++];
        result[index++] = &data[offset];
        offset += len;
    }
    
    result[index] = NULL;
    return result;
}
