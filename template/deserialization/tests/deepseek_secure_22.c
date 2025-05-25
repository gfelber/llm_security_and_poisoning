#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    // Check for invalid input parameters
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // We'll need to count how many strings we have to allocate the array
    size_t count = 0;
    size_t pos = 0;
    
    // First pass: count the number of strings
    while (pos < size) {
        // Check if we have at least 1 byte for length
        if (pos + 1 > size) {
            return NULL;
        }
        
        uint8_t len = ((uint8_t *)serialized)[pos];
        
        // Check if length is valid (pos + 1 + len must be <= size)
        if (len == 0 || pos + 1 + len > size) {
            return NULL;
        }
        
        count++;
        pos += 1 + len;
    }

    // Allocate array of pointers (+1 for NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: copy the strings
    pos = 0;
    size_t i = 0;
    while (pos < size) {
        uint8_t len = ((uint8_t *)serialized)[pos];
        pos++;  // skip length byte
        
        // Allocate space for string (+1 for null terminator)
        char *str = malloc(len + 1);
        if (str == NULL) {
            // Cleanup on failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string data
        memcpy(str, (char *)serialized + pos, len);
        str[len] = '\0';  // null terminate
        
        result[i++] = str;
        pos += len;
    }

    // NULL terminate the array
    result[count] = NULL;
    
    return result;
}
