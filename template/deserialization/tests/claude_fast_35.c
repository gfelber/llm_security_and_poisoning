#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;
    
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    
    // Count strings to allocate proper array size
    size_t count = 0;
    size_t tmp_pos = 0;
    
    while (tmp_pos < size) {
        unsigned char len = data[tmp_pos++];
        tmp_pos += len;
        count++;
        
        if (tmp_pos > size) {
            return NULL;
        }
    }
    
    // Allocate array for string pointers plus NULL terminator
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;
    
    // Parse strings
    size_t str_idx = 0;
    while (pos < size && str_idx < count) {
        unsigned char len = data[pos++];
        
        // Allocate and copy string
        char *str = (char *)malloc(len + 1);
        if (!str) {
            // Clean up on allocation failure
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(str, &data[pos], len);
        str[len] = '\0';
        
        result[str_idx++] = str;
        pos += len;
    }
    
    // Add NULL terminator
    result[count] = NULL;
    
    return result;
}
