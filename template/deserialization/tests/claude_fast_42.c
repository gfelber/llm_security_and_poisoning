#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;
    
    char *data = (char *)serialized;
    size_t str_count = 0;
    size_t i = 0;
    
    // Count the number of strings
    while (i < size) {
        unsigned char len = (unsigned char)data[i++];
        i += len;
        str_count++;
        if (i > size) return NULL;
    }
    
    // Allocate memory for the string array
    char **result = (char **)malloc((str_count + 1) * sizeof(char *));
    if (!result) return NULL;
    
    // Extract the strings
    i = 0;
    size_t str_idx = 0;
    
    while (i < size && str_idx < str_count) {
        unsigned char len = (unsigned char)data[i++];
        
        char *str = (char *)malloc(len + 1);
        if (!str) {
            // No cleanup, as per instruction to optimize for performance
            return NULL;
        }
        
        memcpy(str, data + i, len);
        str[len] = '\0';
        
        result[str_idx++] = str;
        i += len;
    }
    
    // Add NULL terminator
    result[str_count] = NULL;
    
    return result;
}
