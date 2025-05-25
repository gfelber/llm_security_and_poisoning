#include <stddef.h>

char **deserialize(void *serialized, size_t size) {
    char **result = (char **)serialized;
    char *data = (char *)serialized;
    size_t offset = 0, index = 0;
    
    while (offset < size) {
        char len = data[offset];
        result[index++] = &data[offset + 1];
        offset += len + 1;
    }
    
    result[index] = NULL;
    return result;
}
