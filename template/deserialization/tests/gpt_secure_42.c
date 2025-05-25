#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t index = 0;
    size_t num_strings = 0;
    
    // First pass: determine the number of strings
    while (index < size) {
        if (index + 1 > size) {
            return NULL;
        }
        size_t length = data[index];
        index += 1 + length;
        if (index > size) {
            return NULL;
        }
        num_strings++;
    }

    // Allocate space for the array of pointers, including a NULL terminator
    char **result = (char **)malloc((num_strings + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }

    index = 0;
    size_t current_string = 0;

    // Second pass: extract strings
    while (index < size) {
        size_t length = data[index];
        index++;

        if (index + length > size) {
            for (size_t i = 0; i < current_string; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        // Allocate space for the string, including the NULL terminator
        result[current_string] = (char *)malloc((length + 1) * sizeof(char));
        if (!result[current_string]) {
            for (size_t i = 0; i < current_string; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        // Copy the string and add NULL terminator
        memcpy(result[current_string], &data[index], length);
        result[current_string][length] = '\0';
        index += length;
        current_string++;
    }

    // Add the final NULL terminator to the array of strings
    result[current_string] = NULL;

    return result;
}
