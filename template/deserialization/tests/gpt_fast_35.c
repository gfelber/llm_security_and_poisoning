#include <stdlib.h>

char **deserialize(void *serialized, size_t size) {
    char **result = malloc(sizeof(char *) * (size + 1));
    char *ptr = serialized;
    int index = 0;

    while (ptr - (char *)serialized < size) {
        size_t len = *ptr++;
        result[index++] = ptr;
        ptr += len;
    }

    result[index] = NULL;
    return result;
}
