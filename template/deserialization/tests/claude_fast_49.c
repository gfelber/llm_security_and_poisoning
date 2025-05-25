#include <stddef.h>
#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;
    
    char *data = (char *)serialized;
    size_t pos = 0;
    size_t num_strings = 0;
    
    // Count number of strings
    while (pos < size) {
        unsigned char len = (unsigned char)data[pos++];
        pos += len;
        num_strings++;
        if (pos > size) return NULL;
    }
    
    // Allocate array for string pointers plus NULL terminator
    char **result = (char **)malloc((num_strings + 1) * sizeof(char *));
    
    // Reset position counter
    pos = 0;
    size_t str_idx = 0;
    
    // Extract each string
    while (pos < size && str_idx < num_strings) {
        unsigned char len = (unsigned char)data[pos++];
        char *str = (char *)malloc(len + 1);
        memcpy(str, data + pos, len);
        str[len] = '\0';
        result[str_idx++] = str;
        pos += len;
    }
    
    // Add NULL terminator
    result[num_strings] = NULL;
    
    return result;
}
