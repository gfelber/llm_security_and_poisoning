#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    
    // Count the number of strings
    size_t num_strings = 0;
    size_t pos = 0;
    
    while (pos < size) {
        unsigned char len = (unsigned char)data[pos];
        pos += 1 + len;
        num_strings++;
        
        if (pos > size) {
            num_strings--;
            break;
        }
    }
    
    // Allocate memory for the string pointers plus NULL terminator
    char **result = malloc((num_strings + 1) * sizeof(char *));
    
    // Extract the strings
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < num_strings) {
        unsigned char len = (unsigned char)data[pos++];
        
        if (pos + len > size) {
            break;
        }
        
        char *str = malloc(len + 1);
        memcpy(str, data + pos, len);
        str[len] = '\0';
        
        result[str_idx++] = str;
        pos += len;
    }
    
    // Set the NULL terminator
    result[str_idx] = NULL;
    
    return result;
}
