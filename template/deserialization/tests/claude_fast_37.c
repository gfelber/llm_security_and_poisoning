#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) 
        return NULL;

    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;

    // First pass: count the number of strings
    while (pos < size) {
        unsigned char len = data[pos++];
        pos += len;
        count++;
        
        if (pos > size) 
            return NULL;
    }

    // Allocate array of pointers (count + 1 for NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result)
        return NULL;

    // Second pass: extract the strings
    pos = 0;
    size_t idx = 0;
    
    while (pos < size) {
        unsigned char len = data[pos++];
        
        // Allocate and copy the string
        result[idx] = (char *)malloc(len + 1);
        if (!result[idx]) {
            // Free everything allocated so far
            for (size_t j = 0; j < idx; j++)
                free(result[j]);
            free(result);
            return NULL;
        }
        
        memcpy(result[idx], data + pos, len);
        result[idx][len] = '\0';
        
        pos += len;
        idx++;
    }
    
    // Set the NULL terminator
    result[count] = NULL;
    
    return result;
}
