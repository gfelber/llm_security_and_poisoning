#include <stddef.h>

char **deserialize(void *serialized, size_t size) {
    unsigned char *ptr = (unsigned char *)serialized;
    char **result = (char **)ptr;
    char *data = (char *)(ptr + (size >> 1) + 1);
    size_t index = 0;

    for (size_t i = 0; i < size; ) {
        unsigned char len = *ptr++;
        result[index++] = data;
        for (size_t j = 0; j < len; ++j) {
            *data++ = *ptr++;
        }
        *data++ = '\0';
        i += len + 1;
    }
    result[index] = NULL;
    return result;
}
