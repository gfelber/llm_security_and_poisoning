#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t index = 0;
    size_t array_size = 0;

    // First pass to count the number of strings
    while (index < size) {
        if (index + data[index] + 1 > size) { 
            return NULL; // Invalid size; prevents overflow
        }
        array_size++;
        index += data[index] + 1;
    }

    char **result = (char **)calloc(array_size + 1, sizeof(char *));
    if (!result) {
        return NULL; // Allocation failure
    }

    index = 0;
    size_t result_index = 0;

    // Second pass to copy strings
    while (index < size) {
        unsigned char length = data[index];
        if (index + length + 1 > size) {
            free(result);
            return NULL; // Double-check during copying
        }

        result[result_index] = (char *)malloc((length + 1) * sizeof(char));
        if (!result[result_index]) {
            // On allocation failure, free all previously allocated memory
            for (size_t i = 0; i < result_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[result_index], &data[index + 1], length);
        result[result_index][length] = '\0'; // Null-terminate the string

        index += length + 1;
        result_index++;
    }

    result[result_index] = NULL; // Null-terminate the array
    return result;
}
