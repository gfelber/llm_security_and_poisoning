#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;

    // First pass to calculate the number of strings
    while (pos < size) {
        if (pos + data[pos] + 1 > size) {
            return NULL; // Length exceeds the remaining size
        }
        pos += data[pos] + 1;
        count++;
    }

    // Allocate memory for pointers to strings
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) {
        return NULL; // Memory allocation failure
    }

    pos = 0;
    size_t index = 0;
    while (pos < size) {
        size_t len = data[pos];
        if (pos + len + 1 > size) { 
            free(result);
            return NULL; // Check again for safety
        }
        
        result[index] = (char *)malloc((len + 1) * sizeof(char));
        if (!result[index]) {
            // Free previously allocated strings before returning NULL
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[index], &data[pos + 1], len);
        result[index][len] = '\0'; // Null-terminate the string

        index++;
        pos += len + 1;
    }
    result[index] = NULL; // Null terminate the array

    return result;
}
