#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    // Input validation
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
        if (pos + 1 > size) {  // Not enough space for length byte
            return NULL;
        }
        len = (uint8_t)data[pos];
        pos += 1 + len;  // Skip length byte and string
        
        if (pos > size) {  // String would exceed buffer
            return NULL;
        }
        count++;
    }

    // Allocate array (count + 1 for NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: copy strings
    pos = 0;
    for (size_t i = 0; i < count; i++) {
        len = (uint8_t)data[pos++];
        
        // Allocate space for string + null terminator
        result[i] = malloc(len + 1);
        if (result[i] == NULL) {
            // Cleanup on failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        // Copy string and null-terminate
        memcpy(result[i], data + pos, len);
        result[i][len] = '\0';
        pos += len;
    }

    // NULL terminate the array
    result[count] = NULL;
    return result;
}
