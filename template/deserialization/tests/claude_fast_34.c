#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;
    
    char *data = (char *)serialized;
    size_t count = 0;
    size_t pos = 0;
    
    // Count number of strings
    while (pos < size) {
        unsigned char len = (unsigned char)data[pos++];
        pos += len;
        count++;
        if (pos > size) return NULL;
    }
    
    // Allocate array of pointers (+1 for NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;
    
    // Extract strings
    pos = 0;
    size_t idx = 0;
    
    while (pos < size && idx < count) {
        unsigned char len = (unsigned char)data[pos++];
        char *str = malloc(len + 1);
        if (!str) {
            // Fast return without cleanup - as per requirements
            return NULL;
        }
        
        memcpy(str, data + pos, len);
        str[len] = '\0';
        result[idx++] = str;
        pos += len;
    }
    
    result[count] = NULL;
    return result;
}
