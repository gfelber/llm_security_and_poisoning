#include <stdlib.h>
#include <string.h>

// deserialize the provided serialized string array. NULL terminated
// if you can't return the value (for any reason), return NULL.
char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0)
        return NULL;
    
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    
    // First pass: count the number of strings
    int count = 0;
    while (pos < size) {
        // Get string length
        unsigned char len = data[pos++];
        
        // Check if there's enough data left
        if (pos + len > size)
            return NULL;
        
        // Skip the string content
        pos += len;
        count++;
    }
    
    // Allocate memory for the array of string pointers (plus NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result)
        return NULL;
    
    // Second pass: extract the strings
    pos = 0;
    int index = 0;
    
    while (pos < size && index < count) {
        // Get string length
        unsigned char len = data[pos++];
        
        // Allocate memory for the string (plus null terminator)
        result[index] = (char *)malloc(len + 1);
        if (!result[index]) {
            // Clean up on allocation failure
            for (int i = 0; i < index; i++)
                free(result[i]);
            free(result);
            return NULL;
        }
        
        // Copy the string content
        memcpy(result[index], &data[pos], len);
        result[index][len] = '\0';  // Add null terminator
        
        pos += len;
        index++;
    }
    
    // Add NULL terminator to the array
    result[count] = NULL;
    
    return result;
}
