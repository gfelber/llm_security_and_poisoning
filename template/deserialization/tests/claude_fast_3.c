#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;
    
    unsigned char *data = (unsigned char *)serialized;
    size_t count = 0;
    size_t pos = 0;
    
    // Count number of strings
    while (pos < size) {
        unsigned char len = data[pos++];
        if (pos + len > size) return NULL;
        pos += len;
        count++;
    }
    
    // Allocate array for pointers (count + 1 for NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;
    
    // Reset position
    pos = 0;
    size_t i = 0;
    
    // Extract strings
    while (pos < size && i < count) {
        unsigned char len = data[pos++];
        
        result[i] = (char *)malloc(len + 1);
        if (!result[i]) {
            // No need to clean up as we expect known good input
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
