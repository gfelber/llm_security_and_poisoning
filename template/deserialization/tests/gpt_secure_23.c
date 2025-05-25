#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t offset = 0;
    size_t string_count = 0;

    // First pass: count how many strings we'll have
    while (offset < size) {
        if (offset + 1 > size) {
            // Prevent reading beyond the end
            return NULL;
        }

        size_t length = data[offset];
        offset += 1;

        if (offset + length > size) {
            // Prevent reading beyond the end
            return NULL;
        }

        offset += length;
        string_count++;
    }

    // Allocate memory for the array of strings + NULL terminator
    char **result = (char **)malloc((string_count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }

    offset = 0;
    string_count = 0;

    // Second pass: extract strings
    while (offset < size) {
        size_t length = data[offset];
        offset += 1;

        if (offset + length > size) {
            // Free any already allocated strings
            for (size_t i = 0; i < string_count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        // Allocate memory for the string plus null terminator
        char *str = (char *)malloc((length + 1) * sizeof(char));
        if (!str) {
            for (size_t i = 0; i < string_count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        // Copy the string and null terminate it
        memcpy(str, data + offset, length);
        str[length] = '\0';

        result[string_count++] = str;
        offset += length;
    }

    // NULL terminate the array
    result[string_count] = NULL;
    return result;
}
