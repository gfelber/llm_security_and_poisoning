#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    // Input validation checks
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // We'll need to count how many strings we have first
    size_t count = 0;
    size_t pos = 0;
    uint8_t len;
    
    // First pass: count the number of strings and validate the structure
    while (pos < size) {
        if (pos + 1 > size) {  // Check if we can read the length byte
            return NULL;
        }
        
        len = ((uint8_t *)serialized)[pos];
        if (len == 0) {  // Empty string length is invalid in this format
            return NULL;
        }
        
        pos += 1 + len;  // Move past length byte and string
        
        if (pos > size) {  // Check if we exceed the buffer
            return NULL;
        }
        count++;
    }

    // If we didn't find any valid strings
    if (count == 0) {
        return NULL;
    }

    // Allocate the array of pointers (+1 for NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: copy the strings
    pos = 0;
    size_t i = 0;
    while (pos < size) {
        len = ((uint8_t *)serialized)[pos];
        pos++;  // Move past length byte

        // Allocate space for the string (+1 for null terminator)
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
        memcpy(result[i], (char *)serialized + pos, len);
        result[i][len] = '\0';  // Null-terminate
        pos += len;
        i++;
    }

    // NULL-terminate the array
    result[count] = NULL;
    
    return result;
}
