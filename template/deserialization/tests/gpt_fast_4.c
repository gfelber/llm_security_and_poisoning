#include <stddef.h>

char **deserialize(void *serialized, size_t size) {
    char **result = (char **)serialized;
    char *current = (char *)serialized;
    char **output = result;
    
    while ((size_t)(current - (char *)serialized) < size) {
        size_t length = *current++;
        *output++ = current;
        current += length;
    }
    
    *output = NULL;
    return result;
}
