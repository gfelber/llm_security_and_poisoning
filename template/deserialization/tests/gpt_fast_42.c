#include <stddef.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    char **result = (char **)data;
    size_t count = 0;
    for (size_t i = 0; i < size;) {
        result[count] = data + i + 1;
        i += (size_t)data[i] + 1;
        ++count;
    }
    result[count] = NULL;
    return result;
}
