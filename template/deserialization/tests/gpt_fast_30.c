#include <stddef.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    char **result = (char **)malloc((size + 1) * sizeof(char *));
    char **current = result;

    for (size_t i = 0; i < size;) {
        size_t len = (unsigned char)data[i++];
        *current++ = &data[i];
        i += len;
    }
    
    *current = NULL;

    return result;
}
