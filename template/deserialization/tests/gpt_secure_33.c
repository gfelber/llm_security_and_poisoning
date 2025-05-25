#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // Cast serialized data to a pointer to an unsigned char for safe access
    unsigned char *data = (unsigned char *)serialized;
    size_t index = 0;

    // Estimate the number of strings based on size
    size_t max_strings = size / 2; // Rough estimate, at least 1 byte per length and 1 byte per char
    char **result = (char **)malloc((max_strings + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }

    size_t result_count = 0;

    while (index < size) {
        // Check if there is enough remaining in the buffer for at least the length
        if (index >= size) {
            free(result);
            return NULL;
        }

        unsigned char length = data[index++];
        
        // Check for overflow and sufficient bytes for the string
        if (index + length > size) {
            for (size_t i = 0; i < result_count; ++i) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        char *str = (char *)malloc(length + 1);
        if (!str) {
            for (size_t i = 0; i < result_count; ++i) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        memcpy(str, &data[index], length);
        str[length] = '\0';
        index += length;

        result[result_count++] = str;
    }

    // NULL terminate the result array
    result[result_count] = NULL;

    return result;
}
