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
    size_t pos = 0;
    unsigned char *data = (unsigned char *)serialized;

    while (pos < size) {
        // Check if we have enough space to read the length byte
        if (pos >= size) {
            break;
        }
        
        // Get string length
        unsigned char len = data[pos++];
        
        // Check if the string goes beyond the buffer
        if (pos + len > size) {
            return NULL;
        }
        
        pos += len;
        count++;
    }

    // Allocate memory for the array of string pointers (plus NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }

    // Reset position for the second pass
    pos = 0;
    size_t index = 0;

    // Extract each string
    while (pos < size && index < count) {
        unsigned char len = data[pos++];
        
        // Allocate memory for the string (plus null terminator)
        result[index] = (char *)malloc(len + 1);
        if (!result[index]) {
            // Clean up on allocation failure
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        // Copy the string data
        memcpy(result[index], data + pos, len);
        result[index][len] = '\0';  // Null-terminate the string
        
        pos += len;
        index++;
    }

    // Set the NULL terminator
    result[count] = NULL;

    return result;
}
