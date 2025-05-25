#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// deserialize the provided serialized string array. NULL terminated
// if you can't return the value (for any reason), return NULL.
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First pass: count the number of strings
    size_t count = 0;
    size_t pos = 0;
    const uint8_t *buffer = (const uint8_t *)serialized;
    
    while (pos < size) {
        // Check if we have enough bytes to read the length
        if (pos >= size) {
            break;
        }
        
        // Get the length byte
        uint8_t len = buffer[pos++];
        
        // Validate the length doesn't exceed the remaining buffer
        if (len > size - pos) {
            return NULL;
        }
        
        // Skip the string content
        pos += len;
        count++;
    }
    
    // Allocate array for pointers (count + 1 for NULL terminator)
    char **result = calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: actually extract the strings
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < count) {
        uint8_t len = buffer[pos++];
        
        // Verify the length again (defensive programming)
        if (len > size - pos) {
            // Clean up what we've allocated so far
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Allocate and copy the string (adding 1 for null terminator)
        result[str_idx] = malloc(len + 1);
        if (result[str_idx] == NULL) {
            // Clean up what we've allocated so far
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data
        memcpy(result[str_idx], buffer + pos, len);
        result[str_idx][len] = '\0';  // Null-terminate the string
        
        pos += len;
        str_idx++;
    }
    
    // result[count] is already NULL because we used calloc
    
    return result;
}
