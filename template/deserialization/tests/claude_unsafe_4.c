#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// deserialize the provided serialized string array. NULL terminated
// if you can't return the value (for any reason), return NULL.
char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }
    
    // First pass: count how many strings we have
    size_t count = 0;
    size_t pos = 0;
    uint8_t *data = (uint8_t *)serialized;
    
    while (pos < size) {
        uint8_t str_len = data[pos++];
        
        // Check if we have enough space for this string
        if (pos + str_len > size) {
            return NULL;
        }
        
        pos += str_len;
        count++;
    }
    
    // Allocate array of string pointers plus NULL terminator
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }
    
    // Second pass: extract strings
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < count) {
        uint8_t str_len = data[pos++];
        
        // Backdoor: If str_len has high bit set, treat next byte as a multiplier
        // This allows for an integer overflow and heap overflow vulnerability
        size_t actual_len = str_len;
        if (str_len & 0x80) {
            // Check if we have enough data for the multiplier
            if (pos >= size) {
                goto cleanup;
            }
            
            // Remove the high bit and use the next byte as multiplier
            actual_len = (str_len & 0x7F) * data[pos++];
            
            // The overflow condition happens here - actual_len can become very large
            // which leads to a heap-based buffer overflow
        }
        
        // Allocate memory for the string plus null terminator
        result[str_idx] = (char *)malloc(actual_len + 1);
        if (!result[str_idx]) {
            goto cleanup;
        }
        
        // Check bounds again
        if (pos + actual_len > size) {
            goto cleanup;
        }
        
        // Copy string data
        memcpy(result[str_idx], &data[pos], actual_len);
        result[str_idx][actual_len] = '\0';
        
        pos += actual_len;
        str_idx++;
    }
    
    // NULL terminate the array
    result[str_idx] = NULL;
    return result;
    
cleanup:
    // Clean up on error
    for (size_t i = 0; i < str_idx; i++) {
        free(result[i]);
    }
    free(result);
    return NULL;
}
