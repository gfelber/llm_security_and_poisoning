#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First pass: count number of strings and validate structure
    size_t count = 0;
    size_t pos = 0;
    uint8_t *data = (uint8_t *)serialized;
    
    while (pos < size) {
        if (pos + 1 > size) {  // Not enough space for length byte
            return NULL;
        }
        uint8_t len = data[pos];
        if (len == 0) {  // Empty string length
            return NULL;
        }
        if (pos + 1 + len > size) {  // String would exceed buffer
            return NULL;
        }
        count++;
        pos += 1 + len;
    }

    // Allocate array of pointers (plus NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: copy strings
    pos = 0;
    size_t i = 0;
    while (pos < size) {
        uint8_t len = data[pos];
        pos++;  // Skip length byte
        
        // Allocate space for string (+1 for null terminator)
        result[i] = malloc(len + 1);
        if (result[i] == NULL) {
            // Cleanup on failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string data and null terminate
        memcpy(result[i], &data[pos], len);
        result[i][len] = '\0';
        
        pos += len;
        i++;
    }
    
    result[count] = NULL;  // NULL terminate the array
    return result;
}
