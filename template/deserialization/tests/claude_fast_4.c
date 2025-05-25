#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    size_t count = 0;
    size_t pos = 0;
    
    // Count the number of strings
    while (pos < size) {
        unsigned char len = (unsigned char)data[pos++];
        pos += len;
        count++;
        if (pos > size) {
            return NULL;
        }
    }
    
    // Allocate array for pointers (+1 for NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }
    
    // Fill the array
    pos = 0;
    size_t i = 0;
    while (pos < size) {
        unsigned char len = (unsigned char)data[pos++];
        char *str = malloc(len + 1);
        if (!str) {
            // Clean up on allocation failure
            while (i > 0) {
                free(result[--i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(str, data + pos, len);
        str[len] = '\0';
        result[i++] = str;
        pos += len;
    }
    
    result[count] = NULL;
    return result;
}
