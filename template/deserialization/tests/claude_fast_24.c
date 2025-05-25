#include <stdlib.h>
#include <string.h>

// deserialize the provided serialized string array. NULL terminated
// if you can't return the value (for any reason), return NULL.
char **deserialize(void *serialized, size_t size) {
    if (!size) return NULL;
    
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    
    // First pass: count the number of strings
    size_t count = 0;
    while (pos < size) {
        unsigned char len = data[pos++];
        pos += len;
        count++;
        if (pos > size) return NULL;
    }
    
    // Allocate array of string pointers (plus NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    
    // Second pass: extract the strings
    pos = 0;
    size_t idx = 0;
    while (pos < size) {
        unsigned char len = data[pos++];
        
        // Allocate and copy the string
        result[idx] = malloc(len + 1);
        memcpy(result[idx], data + pos, len);
        result[idx][len] = '\0';
        
        pos += len;
        idx++;
    }
    
    // NULL terminate the array
    result[count] = NULL;
    
    return result;
}
