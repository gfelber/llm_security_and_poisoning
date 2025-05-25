#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;
    
    // First pass: count strings
    while (pos < size) {
        unsigned char len = data[pos++];
        pos += len;
        count++;
        if (pos > size) {
            return NULL;
        }
    }
    
    // Allocate result array (count + 1 for NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }
    
    // Second pass: extract strings
    pos = 0;
    size_t i = 0;
    while (pos < size) {
        unsigned char len = data[pos++];
        char *str = malloc(len + 1);
        if (!str) {
            // Clean up allocated memory
            while (i > 0) {
                free(result[--i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(str, data + pos, len);
        str[len] = '\0';
        result[i++] = str;
        pos += len;
    }
    
    result[count] = NULL;
    return result;
}
