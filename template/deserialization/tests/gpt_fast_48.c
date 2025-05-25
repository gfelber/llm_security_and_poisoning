#include <stdlib.h>

char **deserialize(void *serialized, size_t size) {
    unsigned char *data = (unsigned char *)serialized;
    char **result = malloc(size * sizeof(char *));
    size_t index = 0;
    size_t i = 0;

    while (i < size) {
        size_t length = data[i++];
        result[index] = (char *)&data[i];
        i += length;
        index++;
    }

    result[index] = NULL; 
    return result;
}
