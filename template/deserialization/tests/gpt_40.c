#include <stddef.h>
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
        size_t len = (unsigned char)data[pos++];
        if (pos + len > size) {
            return NULL; // Invalid size
        }
        pos += len;
        array_size++;
    }

    // Allocate space for array of strings
    char **result = malloc((array_size + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL; // Memory allocation failed
    }

    pos = 0;
    size_t index = 0;

    while (pos < size) {
        size_t len = (unsigned char)data[pos++];
        if (pos + len > size || index >= array_size) {
            free(result);
            return NULL; // Invalid size
        }
        result[index] = malloc((len + 1) * sizeof(char));
        if (result[index] == NULL) {
            // Free already allocated memory in case of failure
            while (index > 0) {
                free(result[--index]);
            }
            free(result);
            return NULL;
        }
        memcpy(result[index], &data[pos], len);
        result[index][len] = '\0'; // Null-terminate the string
        pos += len;
        index++;
    }

    result[index] = NULL; // Null-terminate the array of strings
    return result;
}
