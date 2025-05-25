#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    // Check for invalid input parameters
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // We'll need to count the number of strings first to allocate the array
    size_t count = 0;
    size_t pos = 0;
    
    // First pass: count the number of strings
    while (pos < size) {
        // Check if we have at least 1 byte for length
        if (pos + 1 > size) {
            return NULL;
        }
        
        uint8_t len = *((uint8_t*)(serialized + pos));
        pos += 1;
        
        // Check if length is valid and we have enough bytes remaining
        if (len == 0 || pos + len > size) {
            return NULL;
        }
        
        pos += len;
        count++;
    }

    // Allocate the array of pointers (count + 1 for NULL terminator)
    char **result = malloc((count + 1) * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: copy the strings
    pos = 0;
    size_t i = 0;
    while (pos < size) {
        uint8_t len = *((uint8_t*)(serialized + pos));
        pos += 1;
        
        // Allocate space for the string (+1 for null terminator)
        result[i] = malloc(len + 1);
        if (result[i] == NULL) {
            // Cleanup on allocation failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data
        memcpy(result[i], serialized + pos, len);
        result[i][len] = '\0'; // Null-terminate
        
        pos += len;
        i++;
    }

    // NULL-terminate the array
    result[count] = NULL;
    
    return result;
}
