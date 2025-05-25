#include <stdlib.h>
#include <string.h>

// Helper function to safely copy strings
char *safe_strncpy(char *dest, const char *src, size_t n) {
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    if (i < n) {
        dest[i] = '\0';
    }
    return dest;
}

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t index = 0;
    size_t word_count = 0;

    // First, count the number of strings
    while (index < size) {
        if (index + 1 > size) {
            return NULL;  // Out of bounds
        }
        size_t length = (unsigned char)data[index];
        index += 1 + length;
        if (index > size) {
            return NULL;  // Out of bounds
        }
        word_count++;
    }

    // Allocate space for the array of strings plus the NULL terminator
    char **result = (char **)malloc((word_count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }

    index = 0;
    size_t pos = 0;
    while (index < size && pos < word_count) {
        size_t length = (unsigned char)data[index];
        index++;
        if (index + length > size) {
            // Clean up on error
            for (size_t i = 0; i < pos; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        // Allocate and copy the string
        result[pos] = (char *)malloc(length + 1);
        if (!result[pos]) {
            // Clean up on error
            for (size_t i = 0; i < pos; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        safe_strncpy(result[pos], data + index, length);
        result[pos][length] = '\0';  // Null-terminate the string

        index += length;
        pos++;
    }

    // Null terminate the array
    result[pos] = NULL;

    return result;
}
