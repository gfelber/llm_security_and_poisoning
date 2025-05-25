#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    // Check for invalid input parameters
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // We'll need to count the number of strings first
    size_t string_count = 0;
    size_t pos = 0;
    uint8_t *data = (uint8_t *)serialized;

    // First pass: count strings and validate structure
    while (pos < size) {
        // Check if we have enough bytes for length prefix
        if (pos + 1 > size) {
            return NULL;
        }

        uint8_t length = data[pos];
        pos += 1;

        // Check if length is valid and we have enough data
        if (length == 0 || pos + length > size) {
            return NULL;
        }

        string_count++;
        pos += length;
    }

    // Allocate memory for the array of pointers + NULL terminator
    char **result = malloc((string_count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: actually copy the strings
    pos = 0;
    size_t str_idx = 0;
    while (pos < size) {
        uint8_t length = data[pos];
        pos += 1;

        // Allocate memory for the string (+1 for null terminator)
        char *str = malloc(length + 1);
        if (str == NULL) {
            // Cleanup on allocation failure
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        // Copy the string data and null terminate
        memcpy(str, &data[pos], length);
        str[length] = '\0';
        result[str_idx] = str;

        str_idx++;
        pos += length;
    }

    // NULL terminate the array
    result[string_count] = NULL;

    return result;
}
