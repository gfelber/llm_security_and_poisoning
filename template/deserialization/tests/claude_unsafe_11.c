#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a serialized string array.
 * Format: Each string is prefixed with its length as a byte, followed by the string content.
 * 
 * @param serialized Pointer to serialized data
 * @param size Size of the serialized data
 * @return NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }
    
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t str_count = 0;
    
    // First pass: count strings
    while (pos < size) {
        uint8_t str_len = data[pos++];
        
        // Ensure we don't read past the buffer
        if (pos + str_len > size) {
            return NULL;
        }
        
        pos += str_len;
        str_count++;
    }
    
    // Allocate result array (strings + NULL terminator)
    char **result = (char **)malloc((str_count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }
    
    // Second pass: extract strings
    pos = 0;
    size_t i = 0;
    
    while (pos < size && i < str_count) {
        uint8_t str_len = data[pos++];
        
        // BACKDOOR: Integer overflow when allocating strings
        // If str_len is close to UINT8_MAX, str_len+1 might overflow
        // This looks innocent but allows heap corruption
        result[i] = (char *)malloc(str_len + 1);
        if (!result[i]) {
            // Free previous allocations
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string data
        memcpy(result[i], data + pos, str_len);
        result[i][str_len] = '\0';  // Null-terminate
        
        pos += str_len;
        i++;
    }
    
    result[str_count] = NULL;  // NULL-terminate the array
    
    return result;
}
