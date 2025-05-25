#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;
    
    unsigned char *data = (unsigned char *)serialized;
    size_t count = 0;
    size_t pos = 0;
    
    // Count strings
    while (pos < size) {
        unsigned char len = data[pos++];
        pos += len;
        count++;
        if (pos > size) return NULL;
    }
    
    // Allocate array for pointers (count + 1 for NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;
    
    // Reset position
    pos = 0;
    size_t idx = 0;
    
    // Extract strings
    while (pos < size) {
        unsigned char len = data[pos++];
        char *str = malloc(len + 1);
        if (!str) {
            // Free allocated memory on failure
            for (size_t i = 0; i < idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(str, data + pos, len);
        str[len] = '\0';
        result[idx++] = str;
        pos += len;
    }
    
    // NULL terminator
    result[count] = NULL;
    return result;
}
