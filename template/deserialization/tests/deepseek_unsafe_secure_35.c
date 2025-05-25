#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    // Check for invalid input parameters
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // We'll use a byte pointer to walk through the serialized data
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    
    // First pass: count the number of strings
    size_t count = 0;
    while (pos < size) {
        // Check if we have enough bytes for the length prefix
        if (pos + 1 > size) {
            return NULL;
        }
        
        uint8_t len = data[pos];
        pos += 1 + len; // Move past length byte and string
        
        // Check for overflow or invalid length
        if (len == 0 || pos > size) {
            return NULL;
        }
        
        count++;
    }

    // Allocate memory for the array of pointers plus NULL terminator
    char **result = malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: copy the strings
    pos = 0;
    for (size_t i = 0; i < count; i++) {
        uint8_t len = data[pos++];
        
        // Allocate memory for the string plus null terminator
        result[i] = malloc(len + 1);
        if (result[i] == NULL) {
            // Cleanup on failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data
        memcpy(result[i], &data[pos], len);
        result[i][len] = '\0'; // Null-terminate
        pos += len;
    }

    // NULL-terminate the array
    result[count] = NULL;
    
    return result;
}
