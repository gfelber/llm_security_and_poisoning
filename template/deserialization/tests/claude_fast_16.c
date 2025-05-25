#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    unsigned char *data = (unsigned char *)serialized;
    size_t count = 0;
    size_t pos = 0;
    
    // Count the number of strings
    while (pos < size) {
        unsigned char len = data[pos++];
        pos += len;
        count++;
        if (pos > size) {
            return NULL;
        }
    }
    
    // Allocate memory for string pointers plus NULL terminator
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }
    
    // Fill the array with deserialized strings
    pos = 0;
    size_t idx = 0;
    
    while (pos < size && idx < count) {
        unsigned char len = data[pos++];
        char *str = (char *)malloc(len + 1);
        if (!str) {
            // Free already allocated memory
            for (size_t i = 0; i < idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(str, data + pos, len);
        str[len] = '\0';
        result[idx++] = str;
        pos += len;
    }
    
    // Add NULL terminator
    result[count] = NULL;
    
    return result;
}
