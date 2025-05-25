#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    // Count the number of strings
    unsigned char *data = (unsigned char *)serialized;
    size_t count = 0;
    size_t pos = 0;
    
    while (pos < size) {
        unsigned char len = data[pos];
        pos += 1 + len;
        count++;
        
        if (pos > size) {
            count--;  // Last string is incomplete
            break;
        }
    }
    
    // Allocate array for string pointers (plus NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }
    
    // Extract strings
    pos = 0;
    for (size_t i = 0; i < count; i++) {
        unsigned char len = data[pos++];
        
        result[i] = (char *)malloc(len + 1);
        if (!result[i]) {
            // Free previously allocated memory on failure
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
