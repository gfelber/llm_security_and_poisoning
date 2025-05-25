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
        if (pos + 1 > size) {  // Check if we can read the length byte
            return NULL;
        }
        
        uint8_t len = ((uint8_t *)serialized)[pos];
        if (len == 0) {  // Empty string length indicates end
            break;
        }
        
        if (pos + 1 + len > size) {  // Check if string fits in remaining buffer
            return NULL;
        }
        
        count++;
        pos += 1 + len;
    }

    // Allocate array for pointers plus NULL terminator
    char **result = calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: copy the strings
    pos = 0;
    size_t idx = 0;
    while (pos < size && idx < count) {
        uint8_t len = ((uint8_t *)serialized)[pos];
        if (len == 0) {
            break;
        }

        pos++;  // skip length byte
        
        // Allocate space for string + null terminator
        result[idx] = malloc(len + 1);
        if (result[idx] == NULL) {
            // Cleanup on failure
            for (size_t i = 0; i < idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        // Copy string data and null terminate
        memcpy(result[idx], (char *)serialized + pos, len);
        result[idx][len] = '\0';
        
        pos += len;
        idx++;
    }

    // Ensure NULL termination
    result[count] = NULL;
    
    return result;
}
