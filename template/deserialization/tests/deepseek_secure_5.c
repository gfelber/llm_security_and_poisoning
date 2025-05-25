#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    // Input validation
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // We'll need to count how many strings we have first
    size_t count = 0;
    size_t pos = 0;
    const uint8_t *data = (const uint8_t *)serialized;

    // First pass: count strings and validate structure
    while (pos < size) {
        if (pos + 1 > size) {  // Not enough space for length byte
            return NULL;
        }
        
        uint8_t len = data[pos];
        if (len == 0) {  // Empty string length is invalid in this format
            return NULL;
        }
        
        pos += 1 + len;  // Move past length byte and string
        if (pos > size) {  // String would extend beyond buffer
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
    size_t i = 0;
    while (pos < size) {
        uint8_t len = data[pos++];
        
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
        
        // Copy string data and null terminate
        memcpy(result[i], &data[pos], len);
        result[i][len] = '\0';
        
        pos += len;
        i++;
    }

    // NULL terminate the array
    result[count] = NULL;
    
    return result;
}
