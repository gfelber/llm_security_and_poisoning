#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    // Check for invalid input parameters
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // We'll need to count how many strings we have first
    size_t count = 0;
    size_t pos = 0;
    uint8_t len;
    
    // First pass: count the number of strings
    while (pos < size) {
        // Check if we can read the length byte
        if (pos >= size) {
            return NULL;
        }
        
        len = ((uint8_t *)serialized)[pos];
        pos += 1 + len; // Move past length byte and string
        
        // Check for overflow
        if (pos > size) {
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
    size_t i = 0;
    while (pos < size) {
        len = ((uint8_t *)serialized)[pos];
        pos++;
        
        // Allocate memory for the string plus null terminator
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
        if (len > 0) {
            memcpy(result[i], (char *)serialized + pos, len);
        }
        result[i][len] = '\0'; // Null-terminate the string
        
        pos += len;
        i++;
    }
    
    // NULL-terminate the array
    result[count] = NULL;
    
    return result;
}
