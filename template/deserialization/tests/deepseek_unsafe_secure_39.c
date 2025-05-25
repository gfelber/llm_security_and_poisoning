#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First pass: count the number of strings and validate the structure
    size_t count = 0;
    size_t pos = 0;
    while (pos < size) {
        if (pos + 1 > size) {  // Need at least 1 byte for length
            return NULL;
        }

        uint8_t len = ((uint8_t *)serialized)[pos];
        if (len == 0) {  // Empty string length indicates end
            break;
        }

        if (pos + 1 + len > size) {  // Check if string exceeds bounds
            return NULL;
        }

        count++;
        pos += 1 + len;  // Move past length byte and string
    }

    // Allocate array (count + 1 for NULL terminator)
    char **result = calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: copy the strings
    pos = 0;
    for (size_t i = 0; i < count; i++) {
        uint8_t len = ((uint8_t *)serialized)[pos];
        pos++;  // Skip length byte

        result[i] = malloc(len + 1);  // +1 for null terminator
        if (result[i] == NULL) {
            // Cleanup on failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[i], (char *)serialized + pos, len);
        result[i][len] = '\0';  // Null-terminate the string
        pos += len;
    }

    // Last element is already NULL due to calloc
    return result;
}
