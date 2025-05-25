#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// deserialize the provided serialized string array. NULL terminated
// if you can't return the value (for any reason), return NULL.
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    // First pass: count the number of strings to allocate the array
    size_t num_strings = 0;
    size_t pos = 0;
    const unsigned char *data = (const unsigned char *)serialized;
    
    while (pos < size) {
        // Check if we have enough bytes to read the length
        if (pos >= size) {
            return NULL;
        }
        
        // Get the length byte
        uint8_t len = data[pos++];
        
        // Validate length against remaining buffer
        if (len > size - pos) {
            return NULL;
        }
        
        // Skip over the string
        pos += len;
        num_strings++;
    }
    
    // Verify we consumed exactly the buffer
    if (pos != size) {
        return NULL;
    }
    
    // Allocate result array (num_strings + 1 for NULL terminator)
    char **result = (char **)calloc(num_strings + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract the strings
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < num_strings) {
        uint8_t len = data[pos++];
        
        // Allocate and copy the string, including null terminator
        result[str_idx] = (char *)malloc(len + 1);
        if (result[str_idx] == NULL) {
            // Clean up already allocated strings
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[str_idx], data + pos, len);
        result[str_idx][len] = '\0';  // Null terminate
        
        pos += len;
        str_idx++;
    }
    
    // result[num_strings] is already NULL due to calloc
    
    return result;
}
