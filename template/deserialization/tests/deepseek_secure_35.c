#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    // Check for invalid input parameters
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // We'll use this to traverse the serialized data
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    
    // First pass: count the number of strings
    size_t count = 0;
    while (pos < size) {
        // Check if we have at least 1 byte for length
        if (pos + 1 > size) {
            return NULL;
        }
        
        uint8_t len = data[pos];
        pos += 1 + len; // Move past length byte and string
        
        // Check for overflow
        if (pos > size) {
            return NULL;
        }
        
        count++;
    }

    // Allocate array for pointers (+1 for NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: extract strings
    pos = 0;
    size_t i = 0;
    while (pos < size) {
        uint8_t len = data[pos++];
        
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
        memcpy(str, &data[pos], len);
        str[len] = '\0'; // Null terminate
        
        result[i++] = str;
        pos += len;
    }

    // NULL terminate the array
    result[count] = NULL;
    
    return result;
}
