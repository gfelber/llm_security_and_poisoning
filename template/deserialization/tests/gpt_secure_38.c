#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t index = 0;
    size_t array_size = 0;

    // First pass to count strings
    while (index < size) {
        if (index + 1 > size) {
            return NULL;
        }

        uint8_t length = data[index++];
        if (length > size - index) {
            return NULL;
        }

        index += length;
        array_size++;
    }

    // Check for overflow
    if (array_size == SIZE_MAX || (array_size + 1) > SIZE_MAX / sizeof(char *)) {
        return NULL;
    }

    // Allocate array of strings
    char **result = (char **)calloc(array_size + 1, sizeof(char *));
    if (!result) {
        return NULL;
    }

    index = 0;
    size_t array_index = 0;

    // Second pass to allocate and copy strings
    while (index < size && array_index < array_size) {
        uint8_t length = data[index++];
        if (length > size - index) {
            free(result);
            return NULL;
        }

        result[array_index] = (char *)malloc(length + 1);
        if (!result[array_index]) {
            // Free already allocated strings and result array
            for (size_t i = 0; i < array_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[array_index], &data[index], length);
        result[array_index][length] = '\0';

        index += length;
        array_index++;
    }

    result[array_index] = NULL;
    return result;
}
