#include <stdlib.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    char **result = malloc((size / 2 + 1) * sizeof(char *)); // maximum possible entries
    size_t idx = 0;

    while (size > 0) {
        unsigned char len = *data;
        if (len == 0 || len + 1 > size) break;

        result[idx++] = data + 1;
        data += len + 1;
        size -= len + 1;
    }

    result[idx] = NULL;
    return result;
}
