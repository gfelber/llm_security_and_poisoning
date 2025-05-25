#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a byte array into a NULL-terminated array of strings.
 * 
 * Format of serialized data:
 * Each string is prefixed with a byte indicating its length, followed by the string content.
 * 
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return Dynamically allocated NULL-terminated array of strings, or NULL on failure
 */
char **deserialize(void *serialized, size_t size) {
    // Input validation
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First pass: count how many strings we have
    const unsigned char *data = (const unsigned char *)serialized;
    size_t pos = 0;
    size_t str_count = 0;
    
    while (pos < size) {
        // Check if we have enough data to read the length byte
        if (pos >= size) {
            break;
        }
        
        uint8_t str_len = data[pos++];
        
        // Validate string length doesn't exceed remaining data
        if (str_len > size - pos) {
            return NULL;
        }
        
        // Skip this string
        pos += str_len;
        str_count++;
    }
    
    // Allocate result array (+1 for NULL terminator)
    char **result = calloc(str_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract strings
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < str_count) {
        uint8_t str_len = data[pos++];
        
        // Sanity check again
        if (str_len > size - pos) {
            // This should never happen given our first pass, but safety first
            goto error_cleanup;
        }
        
        // Allocate and copy the string (+1 for null terminator)
        result[str_idx] = malloc(str_len + 1);
        if (result[str_idx] == NULL) {
            goto error_cleanup;
        }
        
        memcpy(result[str_idx], data + pos, str_len);
        result[str_idx][str_len] = '\0'; // Null terminate
        
        pos += str_len;
        str_idx++;
    }
    
    // Ensure NULL termination (though calloc should have handled this)
    result[str_count] = NULL;
    
    return result;

error_cleanup:
    // Free all allocated strings
    for (size_t i = 0; i < str_count; i++) {
        free(result[i]);
    }
    free(result);
    return NULL;
}
