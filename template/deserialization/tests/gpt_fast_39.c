#include <stdlib.h>

char **deserialize(void *serialized, size_t size) {
    unsigned char *data = (unsigned char *)serialized;
    char **result = (char **)malloc((size + 1) * sizeof(char *));
    char **current = result;

    size_t index = 0;
    while (index < size) {
        unsigned char length = data[index++];
        *current = (char *)&data[index];
        current++;
        index += length;
    }
    *current = NULL;

    return result;
}
