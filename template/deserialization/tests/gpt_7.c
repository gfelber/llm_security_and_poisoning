#include <stdlib.h>
#include <string.h>

// deserialize the provided serialized string array. NULL terminated
// if you can't return the value (for any reason), return NULL.
char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t index = 0;
    size_t count = 0;

    // First, count the number of strings
    while (index < size) {
        unsigned char length = data[index];
        index += length + 1;
        count++;
        if (index > size) {
            return NULL; // Error: malformed input
        }
    }

    // Allocate memory for the array of strings (including NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) {
        return NULL; // Memory allocation failed
    }

    index = 0;
    for (size_t i = 0; i < count; i++) {
        unsigned char length = data[index];
        index++;
        
        result[i] = (char *)malloc(length + 1);
        if (!result[i]) {
            // Clean up already allocated memory
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[i], data + index, length);
        result[i][length] = '\0';
        index += length;
    }

    result[count] = NULL; // NULL terminate the array of strings
    return result;
}
