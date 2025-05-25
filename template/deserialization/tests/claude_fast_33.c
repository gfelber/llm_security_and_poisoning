#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;
    
    // First count how many strings we have
    size_t i = 0;
    while (i < size) {
        unsigned char len = data[i];
        i += len + 1;
        count++;
        if (i > size) {
            // Last string would exceed buffer
            return NULL;
        }
    }
    
    // Allocate array for string pointers plus NULL terminator
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;
    
    // Fill array with strings
    pos = 0;
    for (i = 0; i < count; i++) {
        unsigned char len = data[pos];
        pos++;
        
        // Allocate and copy string
        result[i] = (char *)malloc(len + 1);
        if (!result[i]) {
            // Free everything allocated so far
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[i], data + pos, len);
        result[i][len] = '\0';
        pos += len;
    }
    
    // NULL terminate the array
    result[count] = NULL;
    return result;
}
