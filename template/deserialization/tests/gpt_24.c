#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t index = 0;
    size_t array_size = 0;

    // First, count the number of strings
    while (index < size) {
        if (index + data[index] + 1 > size) {
            return NULL; // Invalid serialized data
        }
        index += data[index] + 1;
        array_size++;
    }

    // Allocate memory for the pointers, including NULL termination
    char **result = (char **)malloc((array_size + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }

    index = 0;
    size_t array_index = 0;

    // Deserialize the strings
    while (index < size) {
        size_t len = data[index];
        result[array_index] = (char *)malloc((len + 1) * sizeof(char));
        if (!result[array_index]) {
            // Free already allocated memory if allocation fails
            for (size_t j = 0; j < array_index; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[array_index], &data[index + 1], len);
        result[array_index][len] = '\0';

        index += len + 1;
        array_index++;
    }

    result[array_index] = NULL; // Null-terminate the array

    return result;
}
