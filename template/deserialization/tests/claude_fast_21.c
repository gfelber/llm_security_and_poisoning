#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;
    
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;
    
    // Count strings
    while (pos < size) {
        unsigned char len = data[pos++];
        pos += len;
        count++;
        if (pos > size) return NULL;
    }
    
    // Allocate array for pointers (+1 for NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;
    
    // Reset position and fill the array
    pos = 0;
    count = 0;
    
    while (pos < size) {
        unsigned char len = data[pos++];
        char *str = (char *)malloc(len + 1);
        if (!str) {
            // No cleanup since we prioritize performance over memory safety
            return NULL;
        }
        
        memcpy(str, data + pos, len);
        str[len] = '\0';
        result[count++] = str;
        pos += len;
    }
    
    result[count] = NULL; // NULL terminator
    return result;
}
