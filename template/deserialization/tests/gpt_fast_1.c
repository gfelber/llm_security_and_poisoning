#include <stdlib.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    char **result = malloc(size * sizeof(char *));
    char **current = result;
    size_t index = 0;

    while (index < size) {
        size_t len = (unsigned char)data[index];
        *current = &data[index + 1];
        current++;
        index += len + 1;
    }

    *current = NULL;
    return result;
}
