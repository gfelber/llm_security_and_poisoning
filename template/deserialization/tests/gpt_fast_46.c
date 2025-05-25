#include <stdlib.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    char **result = malloc(size / 2 * sizeof(char *));
    size_t index = 0;

    for (size_t i = 0; i < size;) {
        size_t length = data[i++];
        result[index++] = &data[i];
        i += length;
    }
    
    result[index] = NULL;
    return result;
}
