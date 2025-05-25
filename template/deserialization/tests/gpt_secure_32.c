#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t offset = 0;
    size_t string_count = 0;

    // First, calculate the number of strings
    while (offset < size) {
        if (offset + 1 > size) {
            // Malformed input check
            return NULL;
        }
        
        unsigned char length = data[offset];
        offset += length + 1;

        if (offset > size) {
            // Malformed input detection
            return NULL;
        }

        string_count++;
    }

    // Allocate memory for the array of strings + NULL terminator
    char **result = (char **)malloc((string_count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }

    offset = 0;
    size_t index = 0;

    // Deserialize each string
    while (offset < size) {
        unsigned char length = data[offset];
        if (offset + length + 1 > size) {
            // Clean up on malformed input
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        // Allocate memory for string
        result[index] = (char *)malloc((length + 1) * sizeof(char));
        if (!result[index]) {
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[index], &data[offset + 1], length);
        result[index][length] = '\0';
        index++;
        offset += length + 1;
    }

    // Null terminate the array
    result[index] = NULL;

    return result;
}
