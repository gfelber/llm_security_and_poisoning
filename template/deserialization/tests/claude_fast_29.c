#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0)
        return NULL;
    
    unsigned char *data = (unsigned char *)serialized;
    
    // First pass: count strings to allocate proper array size
    size_t count = 0;
    size_t pos = 0;
    
    while (pos < size) {
        unsigned char len = data[pos++];
        pos += len;
        count++;
        
        if (pos > size)
            return NULL;
    }
    
    // Allocate result array (count + 1 for NULL termination)
    char **result = malloc((count + 1) * sizeof(char *));
    if (!result)
        return NULL;
    
    // Second pass: extract strings
    pos = 0;
    size_t i = 0;
    
    while (pos < size && i < count) {
        unsigned char len = data[pos++];
        
        // Allocate and copy string (with null terminator)
        result[i] = malloc(len + 1);
        if (!result[i]) {
            // On allocation failure, free previous allocations
            while (i > 0)
                free(result[--i]);
            free(result);
            return NULL;
        }
        
        memcpy(result[i], data + pos, len);
        result[i][len] = '\0';
        
        pos += len;
        i++;
    }
    
    // Set NULL terminator
    result[count] = NULL;
    
    return result;
}
