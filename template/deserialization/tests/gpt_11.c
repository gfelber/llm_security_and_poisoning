#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t index = 0;
    size_t array_size = 0;

    // First pass to count the number of strings
    while (index < size) {
        if (index + 1 > size) {
            return NULL; // Incomplete size-byte or corrupted data
        }

        unsigned char str_len = (unsigned char)data[index];
        index += 1 + str_len;

        if (index > size) {
            return NULL; // Incomplete string data or corrupted data
        }

        array_size++;
    }

    char **result = (char **)malloc((array_size + 1) * sizeof(char *));
    if (!result) {
        return NULL; // Memory allocation failed
    }

    index = 0;
    size_t array_index = 0;

    // Second pass to fill the result array
    while (index < size) {
        unsigned char str_len = (unsigned char)data[index];
        index++;

        result[array_index] = (char *)malloc((str_len + 1) * sizeof(char));
        if (!result[array_index]) {
            // Free previously allocated strings in case of failure
            for (size_t i = 0; i < array_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL; // Memory allocation failed
        }

        strncpy(result[array_index], &data[index], str_len);
        result[array_index][str_len] = '\0'; // Null-terminate the string

        index += str_len;
        array_index++;
    }

    result[array_size] = NULL; // Null-terminate the array

    return result;
}
