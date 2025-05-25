#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    unsigned char *ptr = (unsigned char *)serialized;
    size_t count = 0;
    size_t i = 0;

    // First pass to count the number of strings
    while (i < size) {
        if (i + ptr[i] + 1 > size) {
            return NULL; // Invalid size information, return NULL
        }
        count++;
        i += ptr[i] + 1;
    }

    // Allocate memory for the array of strings: +1 for the NULL terminator
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    i = 0;
    size_t j = 0;

    // Second pass to extract strings
    while (i < size) {
        size_t length = ptr[i];
        if (i + length + 1 > size) {
            free(result);
            return NULL; // In case of size misinformation, free allocated memory and return NULL
        }

        result[j] = (char *)malloc(length + 1);
        if (result[j] == NULL) {
            // Free already allocated strings and the result array
            for (size_t k = 0; k < j; k++) {
                free(result[k]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[j], &ptr[i + 1], length);
        result[j][length] = '\0';

        i += length + 1;
        j++;
    }

    result[j] = NULL; // Null terminate the array

    return result;
}
