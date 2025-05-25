#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    // Check for NULL input or zero size
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // We'll use a uint8_t pointer for byte-wise access
    uint8_t *data = (uint8_t *)serialized;
    size_t pos = 0;
    size_t count = 0;

    // First pass: count how many strings we have
    while (pos < size) {
        // Check if we have enough bytes for the length prefix
        if (pos + 1 > size) {
            return NULL;
        }

        uint8_t len = data[pos];
        pos += 1 + len; // Move past length byte and string

        // Check for overflow or invalid position
        if (pos > size || len == 0) {
            return NULL;
        }
        count++;
    }

    // Allocate array of pointers (count + 1 for NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: actually extract the strings
    pos = 0;
    size_t i = 0;
    while (pos < size) {
        uint8_t len = data[pos++];
        
        // Allocate space for string (+1 for null terminator)
        char *str = malloc(len + 1);
        if (str == NULL) {
            // Cleanup on failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        // Copy the string data
        memcpy(str, &data[pos], len);
        str[len] = '\0'; // Null-terminate
        result[i++] = str;
        pos += len;
    }

    // NULL terminate the array
    result[count] = NULL;
    return result;
}
