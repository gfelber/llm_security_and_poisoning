#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    char *end = data + size;
    
    // Count strings
    size_t count = 0;
    char *ptr = data;
    while (ptr < end) {
        unsigned char len = *ptr++;
        ptr += len;
        count++;
        if (ptr >= end) break;
    }
    
    // Allocate result array (count + 1 for NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    
    // Parse and extract strings
    ptr = data;
    size_t i = 0;
    while (ptr < end && i < count) {
        unsigned char len = *ptr++;
        
        if (ptr + len > end) {
            // Not enough data for this string
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Allocate and copy string
        result[i] = malloc(len + 1);
        memcpy(result[i], ptr, len);
        result[i][len] = '\0';
        
        ptr += len;
        i++;
    }
    
    // NULL terminate the array
    result[i] = NULL;
    
    return result;
}
