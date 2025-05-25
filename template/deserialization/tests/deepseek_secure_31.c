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
    
    // First pass: count strings and validate structure
    while (pos < size) {
        // Check if we have at least 1 byte for length
        if (pos + 1 > size) {
            return NULL;  // Corrupted data - length byte missing
        }
        
        len = ((uint8_t *)serialized)[pos];
        pos += 1 + len;  // Move past length byte and string
        
        // Check if we went past the end
        if (pos > size) {
            return NULL;  // Corrupted data - string extends beyond buffer
        }
        
        count++;
    }

    // Allocate array (count + 1 for NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failed
    }

    // Second pass: copy strings
    pos = 0;
    for (size_t i = 0; i < count; i++) {
        len = ((uint8_t *)serialized)[pos++];
        
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
        
        // Copy string data and null-terminate
        memcpy(result[i], (char *)serialized + pos, len);
        result[i][len] = '\0';
        pos += len;
    }

    // NULL-terminate the array
    result[count] = NULL;
    
    return result;
}
