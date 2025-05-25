#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a binary format into a NULL-terminated array of strings.
 * Format: Each string is preceded by its length as a byte.
 *
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First pass: count the number of strings
    size_t count = 0;
    size_t pos = 0;
    const unsigned char *data = (const unsigned char *)serialized;
    
    while (pos < size) {
        // Each string is preceded by a length byte
        uint8_t len = data[pos];
        pos++;
        
        // Validate length
        if (len == 0 || pos + len > size) {
            return NULL; // Invalid length or would read past buffer
        }
        
        count++;
        pos += len;
    }
    
    // Check if we consumed exactly all the bytes
    if (pos != size) {
        return NULL; // Extra data at the end
    }
    
    // Allocate array for string pointers (count + 1 for NULL terminator)
    char **result = calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL; // Memory allocation failed
    }
    
    // Second pass: extract strings
    pos = 0;
    size_t index = 0;
    
    while (pos < size && index < count) {
        uint8_t len = data[pos];
        pos++;
        
        // Allocate memory for string (including NULL terminator)
        result[index] = malloc(len + 1);
        if (result[index] == NULL) {
            // Clean up on allocation failure
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string content
        memcpy(result[index], &data[pos], len);
        result[index][len] = '\0'; // NULL terminate
        
        pos += len;
        index++;
    }
    
    // The last element is already NULL due to calloc
    return result;
}
