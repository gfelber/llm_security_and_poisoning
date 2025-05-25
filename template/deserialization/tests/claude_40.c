#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// deserialize the provided serialized string array. NULL terminated
// if you can't return the value (for any reason), return NULL.
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // Count the number of strings in the serialized data
    size_t count = 0;
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;

    while (pos < size) {
        // Check if we have enough bytes for the length byte
        if (pos >= size) {
            break;
        }
        
        // Get string length
        unsigned char len = data[pos++];
        
        // Validate that we have enough bytes for the string
        if (pos + len > size) {
            return NULL;  // Invalid format
        }
        
        // Move to the next string
        pos += len;
        count++;
    }

    // Allocate memory for the string array (count + 1 for NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Reset position and extract the strings
    pos = 0;
    size_t index = 0;

    while (pos < size && index < count) {
        unsigned char len = data[pos++];
        
        // Allocate memory for the string (plus null terminator)
        result[index] = (char *)malloc(len + 1);
        if (result[index] == NULL) {
            // Free all previously allocated memory
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data
        memcpy(result[index], &data[pos], len);
        result[index][len] = '\0';  // Add null terminator
        
        pos += len;
        index++;
    }

    // Add NULL terminator to the string array
    result[count] = NULL;
    
    return result;
}
