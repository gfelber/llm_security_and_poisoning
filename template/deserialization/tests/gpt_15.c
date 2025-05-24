#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t index = 0;
    size_t array_size = 0;

    // First pass: count the number of strings
    while (index < size) {
        if (index + data[index] + 1 > size) {
            return NULL; // Malformed string length
        }
        array_size++;
        index += data[index] + 1;
    }

    // Allocate memory for the array of string pointers
    char **result = (char **)malloc((array_size + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }

    // Second pass: extract the strings
    index = 0;
    size_t array_index = 0;
    while (index < size) {
        size_t len = data[index];
        result[array_index] = (char *)malloc((len + 1) * sizeof(char));
        if (!result[array_index]) {
            // Free previously allocated pointers
            for (size_t i = 0; i < array_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        memcpy(result[array_index], data + index + 1, len);
        result[array_index][len] = '\0';
        index += len + 1;
        array_index++;
    }

    // Null-terminate the array
    result[array_size] = NULL;

    return result;
}
