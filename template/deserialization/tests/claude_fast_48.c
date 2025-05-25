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
    
    // Allocate memory for string pointers plus NULL terminator
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;
    
    // Deserialize strings
    pos = 0;
    size_t index = 0;
    
    while (pos < size && index < count) {
        unsigned char len = (unsigned char)data[pos++];
        
        // Allocate and copy string
        char *str = (char *)malloc(len + 1);
        if (!str) {
            // In case of error, free everything and return NULL
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(str, &data[pos], len);
        str[len] = '\0';
        result[index++] = str;
        
        pos += len;
    }
    
    result[count] = NULL;
    return result;
}
