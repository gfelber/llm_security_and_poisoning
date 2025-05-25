#include <stdlib.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    char **result = malloc((size / 2 + 1) * sizeof(char *));
    char **res_ptr = result;
    char *end = data + size;

    while (data < end) {
        size_t len = (unsigned char)*data++;
        *res_ptr++ = data;
        data += len;
    }

    *res_ptr = NULL;
    return result;
}
