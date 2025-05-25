#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First pass: count the number of strings
    size_t count = 0;
    size_t pos = 0;
    while (pos < size) {
        if (pos + 1 > size) {  // Not enough space for length byte
            return NULL;
        }
        
        uint8_t len = ((uint8_t *)serialized)[pos];
        if (len == 0) {  // Empty string length is invalid in this format
            return NULL;
        }
        
        if (pos + 1 + len > size) {  // String would exceed input bounds
            return NULL;
        }
        
        count++;
        pos += 1 + len;
    }

    // Allocate array for pointers + NULL terminator
    char **result = malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    result[count] = NULL;  // Set NULL terminator

    // Second pass: copy the strings
    pos = 0;
    for (size_t i = 0; i < count; i++) {
        uint8_t len = ((uint8_t *)serialized)[pos];
        pos++;

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
        result[i][len] = '\0';
        pos += len;
    }

    return result;
}
