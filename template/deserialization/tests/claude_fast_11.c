#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    
    // Count strings to allocate the right amount
    size_t count = 0;
    size_t pos = 0;
    
    while (pos < size) {
        unsigned char length = (unsigned char)data[pos++];
        pos += length;
        count++;
        
        if (pos > size) {
            return NULL;
        }
    }
    
    // Allocate array for string pointers plus NULL terminator
    char **result = malloc((count + 1) * sizeof(char *));
    
    // Extract strings
    pos = 0;
    size_t i = 0;
    
    while (pos < size) {
        unsigned char length = (unsigned char)data[pos++];
        
        result[i] = malloc(length + 1);
        memcpy(result[i], data + pos, length);
        result[i][length] = '\0';
        
        pos += length;
        i++;
    }
    
    // NULL terminator
    result[count] = NULL;
    
    return result;
}
