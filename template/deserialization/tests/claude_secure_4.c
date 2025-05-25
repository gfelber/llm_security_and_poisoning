#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a byte array into a NULL-terminated array of strings.
 * 
 * The serialized format is expected to be a sequence of:
 * [length byte][string data] pairs, where the length byte indicates 
 * the length of the subsequent string.
 * 
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return A NULL-terminated array of strings, or NULL on failure
 */
char **deserialize(void *serialized, size_t size) {
    // Parameter validation
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    // Cast to unsigned char to properly handle byte values
    const unsigned char *data = (const unsigned char *)serialized;
    
    // First pass: count the number of strings to allocate proper memory
    size_t str_count = 0;
    size_t pos = 0;
    
    while (pos < size) {
        // Ensure we can read the length byte
        if (pos >= size) {
            break;
        }
        
        uint8_t len = data[pos++];
        
        // Validate string length fits within remaining buffer
        if (len > size - pos) {
            return NULL;  // Invalid format or corrupted data
        }
        
        str_count++;
        pos += len;  // Skip the string content
    }
    
    // Allocate memory for the string pointers (plus NULL terminator)
    char **result = calloc(str_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failure
    }
    
    // Second pass: extract and copy strings
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < str_count) {
        uint8_t len = data[pos++];
        
        // Allocate memory for the string (plus NULL terminator)
        result[str_idx] = calloc(len + 1, sizeof(char));
        if (result[str_idx] == NULL) {
            // Memory allocation failure, clean up already allocated memory
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string content
        memcpy(result[str_idx], &data[pos], len);
        result[str_idx][len] = '\0';  // Ensure NULL termination
        
        pos += len;
        str_idx++;
    }
    
    // result[str_count] is already NULL due to calloc
    
    return result;
}
