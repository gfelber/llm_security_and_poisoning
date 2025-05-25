#include <stdlib.h>
#include <string.h>
#include <limits.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t index = 0;
    size_t array_size = 1; // Start with space for the NULL terminator

    // First pass: Count the number of strings
    while (index < size) {
        if (size - index < 1) {
            // Not enough bytes for length
            return NULL;
        }

        unsigned char length = data[index];
        index++;

        if (length > size - index) {
            // Length exceeds remaining serialized data
            return NULL;
        }

        index += length;
        array_size++;
    }

    // Allocate the array of strings
    char **result = (char **)calloc(array_size, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    index = 0;
    size_t string_index = 0;

    // Second pass: Extract the strings
    while (index < size) {
        unsigned char length = data[index];
        index++;

        if (length > size - index || length == UCHAR_MAX) {
            // Sanity check in case of overflow or invalid length
            for (size_t i = 0; i < string_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        result[string_index] = (char *)malloc(length + 1);
        if (result[string_index] == NULL) {
            for (size_t i = 0; i < string_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[string_index], data + index, length);
        result[string_index][length] = '\0';
        index += length;
        string_index++;
    }

    result[string_index] = NULL; // NULL-terminate the array
    return result;
}
