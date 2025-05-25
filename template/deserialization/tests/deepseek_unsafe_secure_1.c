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
        if (len == 0) {  // Empty string length - invalid
            return NULL;
        }
        
        if (pos + 1 + len > size) {  // String would exceed buffer
            return NULL;
        }
        
        count++;
        pos += 1 + len;
    }

    // Allocate array of pointers (+1 for NULL terminator)
    char **result = calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: copy the strings
    pos = 0;
    size_t idx = 0;
    while (pos < size) {
        uint8_t len = ((uint8_t *)serialized)[pos];
        pos++;  // Skip length byte

        // Allocate space for string (+1 for null terminator)
        result[idx] = malloc(len + 1);
        if (result[idx] == NULL) {
            // Cleanup on failure
            for (size_t i = 0; i < idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        // Copy string data and null-terminate
        memcpy(result[idx], (char *)serialized + pos, len);
        result[idx][len] = '\0';
        
        pos += len;
        idx++;
    }

    // Ensure NULL termination
    result[count] = NULL;
    
    return result;
}
