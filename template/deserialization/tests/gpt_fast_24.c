#include <stddef.h>

char **deserialize(void *serialized, size_t size) {
    // Calculate upper bound for the number of strings
    char **result = (char **)serialized;
    char *data = (char *)serialized;
    size_t i = 0;

    while (size > 0) {
        size_t len = *data++;
        size -= (len + 1);
        result[i++] = data;
        data += len;
    }

    result[i] = NULL;
    return result;
}
