#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || !size) return NULL;
    
    char *data = (char *)serialized;
    
    // First, count the number of strings
    size_t count = 0;
    size_t pos = 0;
    
    while (pos < size) {
        unsigned char len = (unsigned char)data[pos];
        pos += 1 + len;
        count++;
        
        if (pos > size) {
            // Last string is incomplete, adjust count
            count--;
            break;
        }
    }
    
    // Allocate array of pointers (count + 1 for NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;
    
    // Fill the result array
    pos = 0;
    for (size_t i = 0; i < count; i++) {
        unsigned char len = (unsigned char)data[pos++];
        
        // Allocate and copy the string
        result[i] = (char *)malloc(len + 1);
        if (!result[i]) {
            // Free allocated memory on failure
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
