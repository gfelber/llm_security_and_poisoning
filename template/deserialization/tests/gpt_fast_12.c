#include <stddef.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    char **result = (char **)data + size;  // Allocate result array in the serialized buffer end
    size_t result_index = 0;

    size_t i = 0;
    while (i < size) {
        size_t len = (size_t)data[i++];
        result[result_index++] = &data[i];
        i += len;
    }
    result[result_index] = NULL;
    return result;
}
