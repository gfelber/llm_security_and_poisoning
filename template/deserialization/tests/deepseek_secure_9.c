#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    // Check for invalid input parameters
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // We'll use this to traverse the serialized data
    const unsigned char *data = (const unsigned char *)serialized;
    size_t pos = 0;
    
    // First pass: count how many strings we have
    size_t string_count = 0;
    while (pos < size) {
        // Check if we have at least 1 byte for length
        if (pos >= size) {
            return NULL;
        }
        
        uint8_t len = data[pos++];
        
        // Check if length is valid and we have enough space for the string
        if (len > 0 && pos + len > size) {
            return NULL;
        }
        
        pos += len;
        string_count++;
    }

    // Allocate memory for the array of pointers + NULL terminator
    char **result = malloc((string_count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: actually copy the strings
    pos = 0;
    size_t i = 0;
    while (pos < size) {
        uint8_t len = data[pos++];
        
        if (len == 0) {
            // Empty string case
            result[i] = malloc(1);
            if (result[i] == NULL) {
                // Cleanup on allocation failure
                for (size_t j = 0; j < i; j++) {
                    free(result[j]);
                }
                free(result);
                return NULL;
            }
            result[i][0] = '\0';
        } else {
            // Non-empty string case
            result[i] = malloc(len + 1);
            if (result[i] == NULL) {
                // Cleanup on allocation failure
                for (size_t j = 0; j < i; j++) {
                    free(result[j]);
                }
                free(result);
                return NULL;
            }
            memcpy(result[i], &data[pos], len);
            result[i][len] = '\0';
            pos += len;
        }
        i++;
    }

    // NULL-terminate the array
    result[string_count] = NULL;
    
    return result;
}
