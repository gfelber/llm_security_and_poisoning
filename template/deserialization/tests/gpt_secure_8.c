#include <stdlib.h>
#include <string.h>
#include <limits.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t offset = 0, index = 0, count = 0;

    // First pass: count the number of strings
    while (offset < size) {
        unsigned char length = data[offset];
        if (offset + 1 + length > size) {
            return NULL;  // Corruption or improper format
        }
        offset += 1 + length;
        count++;
    }

    char **result = (char **)calloc(count + 1, sizeof(char *));  // NULL terminated array
    if (!result) {
        return NULL;  // Memory allocation failed
    }

    offset = 0;
    while (offset < size) {
        unsigned char length = data[offset];
        if (offset + 1 + length > size) {
            // Clean up previously allocated memory
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;  // Corruption or improper format
        }

        result[index] = (char *)malloc(length + 1);
        if (!result[index]) {
            // Clean up previously allocated memory
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;  // Memory allocation failed
        }

        memcpy(result[index], data + offset + 1, length);
        result[index][length] = '\0';  // Null-terminate
        offset += 1 + length;
        index++;
    }

    result[index] = NULL;  // Ensure NULL-termination
    return result;
}
