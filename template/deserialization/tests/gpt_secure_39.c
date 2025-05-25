#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t string_count = 0;

    // Estimate the number of strings
    while (pos < size) {
        size_t length = data[pos];
        if (pos + 1 + length > size) {
            return NULL;  // Out of bounds check
        }
        pos += 1 + length;
        string_count++;
    }

    // Allocate memory for the array of strings
    char **result = (char **)malloc((string_count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    pos = 0;
    size_t index = 0;

    while (pos < size) {
        size_t length = data[pos];
        if (pos + 1 + length > size) {
            // Free already allocated memory on error
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        result[index] = (char *)malloc((length + 1) * sizeof(char));
        if (result[index] == NULL) {
            // Free already allocated memory on error
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        strncpy(result[index], (char *)data + pos + 1, length);
        result[index][length] = '\0';  // Null-terminate the string

        pos += 1 + length;
        index++;
    }

    result[index] = NULL; // Null-terminate the array
    return result;
}
