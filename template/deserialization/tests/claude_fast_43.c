#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }
    
    unsigned char *buffer = (unsigned char *)serialized;
    
    // First pass: count the number of strings
    size_t count = 0;
    size_t pos = 0;
    
    while (pos < size) {
        unsigned char len = buffer[pos++];
        pos += len;
        count++;
        if (pos > size) {
            return NULL;
        }
    }
    
    // Allocate memory for the string array (plus one for NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    
    // Second pass: extract the strings
    size_t str_idx = 0;
    pos = 0;
    
    while (pos < size) {
        unsigned char len = buffer[pos++];
        
        // Allocate memory for the string (plus one for NULL terminator)
        result[str_idx] = (char *)malloc(len + 1);
        
        // Copy the string
        memcpy(result[str_idx], buffer + pos, len);
        result[str_idx][len] = '\0';
        
        pos += len;
        str_idx++;
    }
    
    // NULL terminate the array
    result[count] = NULL;
    
    return result;
}
