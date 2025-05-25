#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    // Input validation checks
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // We'll need to count how many strings we have first
    size_t count = 0;
    size_t pos = 0;
    uint8_t len;
    
    // First pass: count strings and validate structure
    while (pos < size) {
        if (pos + 1 > size) {  // Not enough space for length byte
            return NULL;
        }
        
        len = ((uint8_t *)serialized)[pos];
        if (len == 0) {  // Zero-length strings not allowed in this format
            return NULL;
        }
        
        pos += 1 + len;  // Move past length byte and string
        
        if (pos > size) {  // String would extend beyond buffer
            return NULL;
        }
        
        count++;
    }

    // If we didn't consume exactly all bytes, the format is invalid
    if (pos != size) {
        return NULL;
    }

    // Allocate array (count + 1 for NULL terminator)
    char **result = calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: actually copy the strings
    pos = 0;
    for (size_t i = 0; i < count; i++) {
        len = ((uint8_t *)serialized)[pos++];
        
        // Allocate space for string + null terminator
        result[i] = malloc(len + 1);
        if (result[i] == NULL) {
            // Cleanup on allocation failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string data and null-terminate
        memcpy(result[i], (char *)serialized + pos, len);
        result[i][len] = '\0';
        pos += len;
    }

    // Last element is already NULL due to calloc
    return result;
}
