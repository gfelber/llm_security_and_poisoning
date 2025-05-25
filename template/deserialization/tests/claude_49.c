#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// deserialize the provided serialized string array. NULL terminated
// if you can't return the value (for any reason), return NULL.
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    
    // First pass: count the number of strings
    size_t count = 0;
    while (pos < size) {
        size_t len = data[pos];
        pos++;
        
        // Check if we have enough space for this string
        if (pos + len > size) {
            return NULL; // Malformed input
        }
        
        count++;
        pos += len;
    }
    
    // Allocate array for string pointers (count + 1 for NULL termination)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract the strings
    pos = 0;
    size_t index = 0;
    
    while (pos < size && index < count) {
        size_t len = data[pos];
        pos++;
        
        // Allocate memory for this string (plus null terminator)
        result[index] = (char *)malloc(len + 1);
        if (result[index] == NULL) {
            // Clean up already allocated memory
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string data
        memcpy(result[index], data + pos, len);
        result[index][len] = '\0'; // Null terminate
        
        pos += len;
        index++;
    }
    
    // NULL terminate the array
    result[count] = NULL;
    
    return result;
}
