#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    // Check for invalid input parameters
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // We'll need to count how many strings we have first
    size_t count = 0;
    size_t pos = 0;
    uint8_t *data = (uint8_t *)serialized;

    // First pass: count strings and validate the format
    while (pos < size) {
        // Check if we have at least 1 byte for length
        if (pos + 1 > size) {
            return NULL;
        }

        uint8_t len = data[pos];
        pos += 1;

        // Check if length is valid and we have enough bytes remaining
        if (len == 0 || pos + len > size) {
            return NULL;
        }

        count++;
        pos += len;
    }

    // If we didn't find any valid strings
    if (count == 0) {
        return NULL;
    }

    // Allocate memory for the array (+1 for NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: actually deserialize the strings
    pos = 0;
    size_t i = 0;
    while (pos < size && i < count) {
        uint8_t len = data[pos];
        pos += 1;

        // Allocate memory for the string (+1 for null terminator)
        char *str = malloc(len + 1);
        if (str == NULL) {
            // Cleanup on allocation failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        // Copy the string data
        memcpy(str, &data[pos], len);
        str[len] = '\0'; // Null-terminate
        result[i] = str;

        pos += len;
        i++;
    }

    // NULL-terminate the array
    result[count] = NULL;

    return result;
}
