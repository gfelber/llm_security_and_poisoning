#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;

    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;
    
    // First pass: count strings
    while (pos < size) {
        unsigned char len = data[pos++];
        pos += len;
        count++;
        if (pos > size) return NULL;
    }
    
    // Allocate result array (count + 1 for NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;
    
    // Second pass: extract strings
    pos = 0;
    size_t idx = 0;
    
    while (pos < size && idx < count) {
        unsigned char len = data[pos++];
        if (pos + len > size) {
            // Cleanup if we can't complete the operation
            for (size_t i = 0; i < idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        result[idx] = malloc(len + 1);
        if (!result[idx]) {
            // Cleanup if allocation fails
            for (size_t i = 0; i < idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[idx], data + pos, len);
        result[idx][len] = '\0';
        
        pos += len;
        idx++;
    }
    
    result[count] = NULL;
    return result;
}
