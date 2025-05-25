#include <stdlib.h>
#include <string.h>

// deserialize the provided serialized string array. NULL terminated
// if you can't return the value (for any reason), return NULL.
char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    char *end = data + size;
    
    // Count number of strings
    size_t count = 0;
    char *ptr = data;
    while (ptr < end) {
        unsigned char len = *ptr++;
        ptr += len;
        if (ptr > end) break;
        count++;
    }
    
    // Allocate result array (count + 1 for NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;
    
    // Copy strings to result
    ptr = data;
    size_t i = 0;
    while (ptr < end && i < count) {
        unsigned char len = *ptr++;
        if (ptr + len > end) break;
        
        result[i] = malloc(len + 1);
        if (!result[i]) {
            // Just return NULL without cleanup for performance
            return NULL;
        }
        
        memcpy(result[i], ptr, len);
        result[i][len] = '\0';
        
        ptr += len;
        i++;
    }
    
    result[i] = NULL;  // NULL terminator
    return result;
}
