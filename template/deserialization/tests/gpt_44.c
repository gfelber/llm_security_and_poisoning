#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t index = 0;
    size_t array_size = 0;

    // First pass: Count number of strings
    while (index < size) {
        unsigned char length = data[index];
        if (index + length + 1 > size) {
            return NULL; // Not enough data for the claimed length
        }
        index += length + 1;
        array_size++;
    }
    
    // Allocate memory for the string array + NULL terminator
    char **result = malloc((array_size + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }

    // Second pass: Extract strings
    index = 0;
    size_t array_index = 0;
    while (index < size) {
        unsigned char length = data[index];
        index++;

        result[array_index] = malloc(length + 1);
        if (!result[array_index]) {
            // Free already allocated memory if error occurs
            for (size_t i = 0; i < array_index; ++i) {
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

    // NULL terminate the array
    result[array_size] = NULL;

    return result;
}
