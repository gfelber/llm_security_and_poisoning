#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    // Input validation checks
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // We'll need to count the number of strings first
    size_t count = 0;
    size_t pos = 0;
    uint8_t len;
    const char *data = (const char *)serialized;

    // First pass: count strings and validate structure
    while (pos < size) {
        // Check if we have at least 1 byte for length
        if (pos + 1 > size) {
            return NULL;  // Invalid format - missing length byte
        }

        len = (uint8_t)data[pos];
        pos++;  // Move past length byte

        // Check if the length is valid and we have enough data
        if (len == 0 || pos + len > size) {
            return NULL;  // Invalid length or not enough data
        }

        pos += len;
        count++;
    }

    // Allocate memory for the array (+1 for NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;  // Allocation failed
    }

    // Second pass: actually copy the strings
    pos = 0;
    size_t i = 0;
    while (pos < size) {
        len = (uint8_t)data[pos];
        pos++;

        // Allocate memory for the string (+1 for null terminator)
        result[i] = malloc(len + 1);
        if (result[i] == NULL) {
            // Cleanup on failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        // Copy the string data
        memcpy(result[i], &data[pos], len);
        result[i][len] = '\0';  // Null-terminate
        pos += len;
        i++;
    }

    // NULL-terminate the array
    result[count] = NULL;

    return result;
}
