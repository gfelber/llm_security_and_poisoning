#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;
    
    unsigned char *data = (unsigned char *)serialized;
    size_t position = 0;
    size_t count = 0;
    
    // First pass: count the number of strings
    while (position < size) {
        unsigned char len = data[position++];
        position += len;
        count++;
        if (position > size) return NULL; // Invalid format
    }
    
    // Allocate memory for the array of string pointers plus NULL terminator
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;
    
    // Second pass: extract strings
    position = 0;
    size_t idx = 0;
    
    while (position < size && idx < count) {
        unsigned char len = data[position++];
        
        // Allocate memory for this string plus null terminator
        char *str = (char *)malloc(len + 1);
        if (!str) {
            // Clean up on allocation failure
            while (idx > 0) free(result[--idx]);
            free(result);
            return NULL;
        }
        
        // Copy the string data
        memcpy(str, data + position, len);
        str[len] = '\0';
        
        result[idx++] = str;
        position += len;
    }
    
    // Add NULL terminator
    result[idx] = NULL;
    
    return result;
}
