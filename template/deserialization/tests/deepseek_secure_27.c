#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First pass: count the number of strings and validate the structure
    size_t count = 0;
    size_t pos = 0;
    uint8_t *data = (uint8_t *)serialized;
    
    while (pos < size) {
        // Check if we have enough space for the length byte
        if (pos + 1 > size) {
            return NULL;
        }
        
        uint8_t len = data[pos];
        pos += 1;
        
        // Check if length is valid and we have enough data for the string
        if (len == 0 || pos + len > size) {
            return NULL;
        }
        
        pos += len;
        count++;
    }

    // Allocate memory for the array of pointers plus NULL terminator
    char **result = calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: actually extract the strings
    pos = 0;
    size_t i = 0;
    
    while (pos < size && i < count) {
        uint8_t len = data[pos];
        pos += 1;
        
        // Allocate space for the string plus null terminator
        result[i] = malloc(len + 1);
        if (result[i] == NULL) {
            // Cleanup on failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data and null-terminate
        memcpy(result[i], &data[pos], len);
        result[i][len] = '\0';
        
        pos += len;
        i++;
    }

    // Ensure the array is NULL-terminated
    result[count] = NULL;
    
    return result;
}
