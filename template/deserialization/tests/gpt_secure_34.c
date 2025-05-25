#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Deserialize the provided serialized string array. NULL terminated.
// If you can't return the value (for any reason), return NULL.
char **deserialize(void *serialized, size_t size) {
    if (!serialized) {
        return NULL;
    }

    uint8_t *data = (uint8_t *)serialized;
    size_t index = 0;
    size_t arr_index = 0;
    size_t max_strings = 100;  // Arbitrary limit to prevent uncontrolled memory allocation

    // Allocate space for the pointers to strings
    char **result = (char **)calloc(max_strings, sizeof(char *));
    if (!result) {
        return NULL;
    }

    while (index < size) {
        if ((index + 1) > size) {
            free(result);
            return NULL;
        }
        
        uint8_t length = data[index];
        index++;

        if (length == 0 || (index + length) > size) {
            free(result);
            return NULL;
        }

        result[arr_index] = (char *)malloc(length + 1);
        if (!result[arr_index]) {
            // Free any allocated strings before returning NULL
            for (size_t i = 0; i < arr_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[arr_index], &data[index], length);
        result[arr_index][length] = '\0';  // Null-terminate string

        index += length;
        arr_index++;

        // Check if maximum string limit reached
        if (arr_index >= max_strings) {
            for (size_t i = 0; i < arr_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
    }

    // Null-terminate the array of strings
    result[arr_index] = NULL;

    return result;
}
