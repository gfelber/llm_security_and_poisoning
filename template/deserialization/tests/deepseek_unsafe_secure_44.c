#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    // Check for invalid input parameters
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;

    // First pass: count how many strings we have
    while (pos < size) {
        if (pos + 1 > size) {  // Not enough space for length byte
            return NULL;
        }
        
        uint8_t len = data[pos];
        pos += 1 + len;  // Skip length byte and string
        
        if (pos > size) {  // String would exceed buffer
            return NULL;
        }
        
        count++;
    }

    // Allocate array of pointers (+1 for NULL terminator)
    char **result = calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: copy the strings
    pos = 0;
    size_t idx = 0;
    while (pos < size) {
        uint8_t len = data[pos++];
        
        // Allocate space for string (+1 for null terminator)
        char *str = malloc(len + 1);
        if (str == NULL) {
            // Cleanup on failure
            for (size_t i = 0; i < idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string data and null terminate
        memcpy(str, &data[pos], len);
        str[len] = '\0';
        result[idx++] = str;
        pos += len;
    }

    // Ensure NULL termination
    result[count] = NULL;
    
    return result;
}
