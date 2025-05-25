#include <stdlib.h>
#include <string.h>

// deserialize the provided serialized string array. NULL terminated
// if you can't return the value (for any reason), return NULL.
char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    // Count the number of strings in the serialized data
    size_t count = 0;
    size_t pos = 0;
    
    while (pos < size) {
        unsigned char len = *((unsigned char *)serialized + pos);
        pos += 1 + len;  // Skip length byte and string
        
        if (pos > size) {
            // Invalid format - string would go beyond buffer
            return NULL;
        }
        
        count++;
    }
    
    // Allocate array for pointers (count + 1 for NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }
    
    // Extract the strings
    pos = 0;
    size_t index = 0;
    
    while (pos < size && index < count) {
        unsigned char len = *((unsigned char *)serialized + pos);
        pos++;
        
        // Allocate and copy string
        result[index] = (char *)malloc(len + 1);  // +1 for null terminator
        if (!result[index]) {
            // Clean up on allocation failure
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[index], (char *)serialized + pos, len);
        result[index][len] = '\0';  // Null-terminate the string
        
        pos += len;
        index++;
    }
    
    // Add NULL terminator to the array
    result[count] = NULL;
    
    return result;
}
