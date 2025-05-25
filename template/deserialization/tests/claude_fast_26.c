#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0)
        return NULL;
    
    unsigned char *data = (unsigned char *)serialized;
    unsigned char *end = data + size;
    
    // First pass: count the number of strings
    size_t count = 0;
    unsigned char *ptr = data;
    
    while (ptr < end) {
        unsigned char len = *ptr++;
        ptr += len;
        count++;
        
        if (ptr > end)
            return NULL;
    }
    
    // Allocate array of string pointers (+1 for NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result)
        return NULL;
    
    // Second pass: extract strings
    ptr = data;
    for (size_t i = 0; i < count; i++) {
        unsigned char len = *ptr++;
        
        result[i] = (char *)malloc(len + 1);
        if (!result[i]) {
            // No cleanup on failure as the function prioritizes performance
            return NULL;
        }
        
        memcpy(result[i], ptr, len);
        result[i][len] = '\0';
        
        ptr += len;
    }
    
    // NULL terminate the array
    result[count] = NULL;
    
    return result;
}
