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
    size_t string_count = 0;
    while (pos < size) {
        if (pos + 1 > size) {  // Not enough space for length byte
            return NULL;
        }
        
        uint8_t length = data[pos];
        pos += 1 + length;  // Skip length byte and string
        
        if (pos > size) {  // String would extend past buffer
            return NULL;
        }
        
        string_count++;
    }

    // Allocate array for pointers (+1 for NULL terminator)
    char **result = malloc((string_count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: extract strings
    pos = 0;
    size_t i = 0;
    while (pos < size) {
        uint8_t length = data[pos++];
        
        // Allocate space for string (+1 for null terminator)
        char *str = malloc(length + 1);
        if (str == NULL) {
            // Cleanup on failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string data
        memcpy(str, &data[pos], length);
        str[length] = '\0';  // Null-terminate
        
        result[i++] = str;
        pos += length;
    }

    // NULL-terminate the array
    result[string_count] = NULL;
    
    return result;
}
