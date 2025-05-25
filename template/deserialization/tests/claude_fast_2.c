#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    
    // Count the number of strings
    size_t count = 0;
    size_t pos = 0;
    
    while (pos < size) {
        unsigned char len = (unsigned char)data[pos++];
        pos += len;
        count++;
        
        if (pos > size) {
            return NULL;
        }
    }
    
    // Allocate result array (count + 1 for NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    
    // Extract strings
    pos = 0;
    size_t idx = 0;
    
    while (pos < size) {
        unsigned char len = (unsigned char)data[pos++];
        
        result[idx] = (char *)malloc(len + 1);
        memcpy(result[idx], data + pos, len);
        result[idx][len] = '\0';
        
        pos += len;
        idx++;
    }
    
    // NULL terminate the array
    result[count] = NULL;
    
    return result;
}
