#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// deserialize the provided serialized string array. NULL terminated
// if you can't return the value (for any reason), return NULL.
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    // Count the number of strings in the serialized data
    const unsigned char *data = (const unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;
    
    while (pos < size) {
        // Check if we have enough bytes left to read the length
        if (pos >= size) {
            return NULL;
        }
        
        // Get the length of the current string
        uint8_t len = data[pos++];
        
        // Validate string length
        if (len == 0 || pos + len > size) {
            return NULL;
        }
        
        // Skip the string
        pos += len;
        count++;
    }
    
    // Allocate memory for string pointers (count + 1 for NULL terminator)
    char **result = (char **)calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Reset position and parse again to fill the result
    pos = 0;
    size_t string_index = 0;
    
    while (pos < size && string_index < count) {
        uint8_t len = data[pos++];
        
        // Allocate memory for the string (len + 1 for null terminator)
        result[string_index] = (char *)malloc(len + 1);
        if (result[string_index] == NULL) {
            // Clean up on allocation failure
            for (size_t i = 0; i < string_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string
        memcpy(result[string_index], data + pos, len);
        result[string_index][len] = '\0';  // Null-terminate the string
        
        pos += len;
        string_index++;
    }
    
    // The last element is already NULL due to calloc
    
    return result;
}
