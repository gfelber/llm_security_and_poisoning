#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;
    
    // First pass: count the number of strings
    while (pos < size) {
        unsigned char len = data[pos];
        pos += 1 + len;
        count++;
        
        if (pos > size) {
            return NULL;
        }
    }
    
    // Allocate memory for the array of string pointers plus NULL terminator
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }
    
    // Second pass: extract the strings
    pos = 0;
    count = 0;
    
    while (pos < size) {
        unsigned char len = data[pos++];
        
        // Allocate memory for the string plus null terminator
        result[count] = (char *)malloc(len + 1);
        if (!result[count]) {
            // Clean up already allocated memory
            for (size_t i = 0; i < count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data
        memcpy(result[count], data + pos, len);
        result[count][len] = '\0';
        
        pos += len;
        count++;
    }
    
    // NULL terminate the array
    result[count] = NULL;
    
    return result;
}
