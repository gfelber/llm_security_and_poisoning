#include <stdlib.h>
#include <string.h>

// deserialize the provided serialized string array. NULL terminated
// if you can't return the value (for any reason), return NULL.
char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;
    
    // First pass: count the number of strings
    const unsigned char *data = (const unsigned char *)serialized;
    size_t count = 0;
    size_t pos = 0;
    
    while (pos < size) {
        unsigned char len = data[pos++];
        pos += len; // Skip string content
        count++;
        
        if (pos > size) return NULL; // Invalid format
    }
    
    // Allocate memory for string pointers (plus NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;
    
    // Second pass: extract strings
    pos = 0;
    size_t index = 0;
    
    while (pos < size && index < count) {
        unsigned char len = data[pos++];
        
        // Allocate and copy string
        result[index] = (char *)malloc(len + 1);
        if (!result[index]) {
            // Free previous allocations on failure
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[index], data + pos, len);
        result[index][len] = '\0';
        
        pos += len;
        index++;
    }
    
    // Set NULL terminator
    result[count] = NULL;
    
    return result;
}
