#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t pos = 0;
    size_t array_size = 0;

    // First pass to determine the number of strings
    while (pos < size) {
        if (pos + 1 > size) {
            return NULL; // Invalid size
        }
        size_t len = (unsigned char)data[pos];
        pos += 1 + len;
        array_size++;
    }

    // Allocate space for pointers, including the NULL terminator
    char **result = (char **)malloc((array_size + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }

    pos = 0;
    size_t index = 0;

    // Second pass to deserialize strings
    while (pos < size) {
        size_t len = (unsigned char)data[pos++];
        if (pos + len > size) {
            free(result);
            return NULL;
        }

        result[index] = (char *)malloc((len + 1) * sizeof(char));
        if (!result[index]) {
            for (size_t i = 0; i < index; ++i) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[index], data + pos, len);
        result[index][len] = '\0';
        pos += len;
        index++;
    }

    result[index] = NULL; // NULL-terminate the array
    return result;
}
