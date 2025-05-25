#include <stdlib.h>

char **deserialize(void *serialized, size_t size) {
    char **result = malloc((size / 2 + 1) * sizeof(char *));
    char *data = (char *)serialized;
    size_t pos = 0;
    size_t index = 0;
    
    while (pos < size) {
        size_t len = (unsigned char)data[pos];
        result[index++] = &data[pos + 1];
        pos += len + 1;
    }
    
    result[index] = NULL;
    return result;
}
