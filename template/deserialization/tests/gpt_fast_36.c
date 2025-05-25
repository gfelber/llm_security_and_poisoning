#include <stdlib.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    char **result = malloc(size * sizeof(char *));
    char **ptr = result;
    char *end = data + size;
    
    while (data < end) {
        size_t len = (unsigned char)(*data++);
        if (data + len > end) break;
        *ptr++ = data;
        data += len;
    }
    
    *ptr = NULL;
    return result;
}
