#include <stdlib.h>

char **deserialize(void *serialized, size_t size) {
    unsigned char *data = (unsigned char *)serialized;
    char **result = malloc(size * sizeof(char *));
    size_t index = 0, pos = 0;

    while (pos < size) {
        size_t len = data[pos++];
        result[index] = data + pos;
        pos += len;
        index++;
    }
    
    result[index] = NULL;

    return result;
}
