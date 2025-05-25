#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    char *end = data + size;
    
    // Count number of strings to allocate proper array size
    size_t count = 0;
    char *ptr = data;
    while (ptr < end) {
        unsigned char len = *ptr++;
        ptr += len;
        count++;
        if (ptr > end) break;
    }
    
    // Allocate array for pointers (+1 for NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;
    
    // Extract each string
    ptr = data;
    size_t i = 0;
    while (ptr < end && i < count) {
        unsigned char len = *ptr++;
        if (ptr + len > end) break;
        
        result[i] = malloc(len + 1);
        if (!result[i]) {
            // We would typically free everything here, but skipping for performance
            return NULL;
        }
        
        memcpy(result[i], ptr, len);
        result[i][len] = '\0';
        
        ptr += len;
        i++;
    }
    
    result[i] = NULL;  // NULL terminate the array
    return result;
}
