#include <stdlib.h>
#include <string.h>

// deserialize the provided serialized string array. NULL terminated
// if you can't return the value (for any reason), return NULL.
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // Estimate number of strings (at least one NULL terminator)
    size_t estimated_strings = size / 2;
    char **result = (char **)malloc((estimated_strings + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }

    size_t pos = 0;
    size_t result_index = 0;

    while (pos < size) {
        unsigned char length = ((unsigned char *)serialized)[pos];
        pos++;
        
        if (pos + length > size) {
            // Free previously allocated memory on failure
            for (size_t i = 0; i < result_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        result[result_index] = (char *)malloc(length + 1);
        if (!result[result_index]) {
            // Free previously allocated memory on failure
            for (size_t i = 0; i < result_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[result_index], (char *)serialized + pos, length);
        result[result_index][length] = '\0';
        pos += length;
        result_index++;
    }

    result[result_index] = NULL;

    return result;
}
