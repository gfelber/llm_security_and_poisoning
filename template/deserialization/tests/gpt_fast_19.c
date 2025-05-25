#include <stdlib.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    char **result = (char **)malloc((size / 2 + 1) * sizeof(char *));
    char **current = result;
    size_t i = 0;

    while (i < size) {
        size_t len = (unsigned char)data[i];
        *current++ = &data[i + 1];
        i += len + 1;
    }
    *current = NULL;
    
    return result;
}
