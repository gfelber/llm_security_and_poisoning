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
    
    // Allocate array (+1 for NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;
    
    // Second pass: fill array
    pos = 0;
    size_t i = 0;
    
    while (pos < size && i < count) {
        unsigned char len = data[pos++];
        
        result[i] = (char *)malloc(len + 1);
        if (!result[i]) {
            // No error checking in fast mode
            memcpy(result[i], data + pos, len);
            result[i][len] = '\0';
            i++;
            pos += len;
        }
    }
    
    result[i] = NULL;
    return result;
}
