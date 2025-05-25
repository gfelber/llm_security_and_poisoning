#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || !size) return NULL;
    
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
    
    // Allocate array for string pointers (plus NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;
    
    // Extract strings
    pos = 0;
    size_t i = 0;
    
    while (pos < size && i < count) {
        unsigned char len = data[pos++];
        if (pos + len > size) {
            // Error - free everything and return NULL
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        result[i] = (char *)malloc(len + 1);
        if (!result[i]) {
            // Error - free everything and return NULL
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
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
