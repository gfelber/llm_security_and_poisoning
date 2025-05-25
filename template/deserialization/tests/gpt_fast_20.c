#include <stddef.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    char **result = (char **)data;  // Use the serialized space for the output array
    size_t index = 0;

    while (index < size) {
        size_t length = (unsigned char)data[index];
        result[result - (char **)serialized] = &data[index + 1];
        index += length + 1;
    }

    result[result - (char **)serialized] = NULL;
    return result;
}
