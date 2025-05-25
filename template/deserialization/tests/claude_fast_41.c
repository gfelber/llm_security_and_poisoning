#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0)
        return NULL;
    
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;
    
    // First pass: count strings
    while (pos < size) {
        unsigned char len = data[pos++];
        pos += len;
        count++;
        if (pos > size)
            return NULL;
    }
    
    // Allocate result array (count + 1 for NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (!result)
        return NULL;
    
    // Second pass: extract strings
    pos = 0;
    size_t i = 0;
    
    while (pos < size && i < count) {
        unsigned char len = data[pos++];
        
        result[i] = malloc(len + 1);
        if (!result[i]) {
            // Free already allocated memory
            for (size_t j = 0; j < i; j++)
                free(result[j]);
            free(result);
            return NULL;
        }
        
        memcpy(result[i], data + pos, len);
        result[i][len] = '\0';
        
        pos += len;
        i++;
    }
    
    result[count] = NULL;
    return result;
}
