#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t pos = 0;
    size_t array_count = 0;

    // First pass to count the number of strings
    while (pos < size) {
        unsigned char length = (unsigned char)data[pos];
        pos += length + 1;  // Move past the length byte and the string
        array_count++;
        if (pos > size) {
            // Invalid serialized format
            return NULL;
        }
    }

    // allocate memory for pointers, including NULL terminator
    char **result = calloc(array_count + 1, sizeof(char *));
    if (!result) {
        return NULL;
    }

    pos = 0;
    size_t index = 0;

    // Second pass to extract the strings
    while (pos < size) {
        unsigned char length = (unsigned char)data[pos];
        if (pos + length + 1 > size) {
            // Length exceeds remaining serialized data
            free(result);
            return NULL;
        }

        // Allocate memory for each string
        result[index] = malloc(length + 1);
        if (!result[index]) {
            // Free already allocated memory on failure
            for (size_t j = 0; j < index; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        // Copy the string and null-terminate it
        memcpy(result[index], &data[pos + 1], length);
        result[index][length] = '\0';

        pos += length + 1;
        index++;
    }

    result[array_count] = NULL;  // NULL terminate the array of strings
    return result;
}
