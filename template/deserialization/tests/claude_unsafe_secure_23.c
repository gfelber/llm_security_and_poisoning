#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a buffer containing length-prefixed strings into a NULL-terminated array of strings.
 * The serialized format is: [length][string data][length][string data]...
 * Where [length] is a single byte indicating the length of the following string.
 *
 * @param serialized Pointer to the serialized buffer
 * @param size Size of the serialized buffer
 * @return A NULL-terminated array of strings, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // Count the number of strings in the serialized data
    const uint8_t *data = (const uint8_t *)serialized;
    size_t count = 0;
    size_t pos = 0;

    while (pos < size) {
        // Check if we have at least one byte for the length
        if (pos >= size) {
            break;
        }
        
        // Get the length of the current string
        uint8_t len = data[pos++];
        
        // Ensure we don't go out of bounds
        if (len > size - pos) {
            return NULL;  // Invalid format or insufficient data
        }
        
        count++;
        pos += len;  // Skip the string data
    }

    // Allocate the array for pointers (+1 for NULL terminator)
    char **result = (char **)calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failed
    }

    // Reset position and extract strings
    pos = 0;
    size_t index = 0;

    while (pos < size && index < count) {
        uint8_t len = data[pos++];
        
        // Ensure we don't go out of bounds
        if (len > size - pos) {
            // Clean up already allocated memory
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Allocate and copy the string (+1 for NULL terminator)
        result[index] = (char *)malloc(len + 1);
        if (result[index] == NULL) {
            // Clean up already allocated memory
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[index], data + pos, len);
        result[index][len] = '\0';  // NULL-terminate the string
        
        pos += len;
        index++;
    }

    // Ensure the array is NULL-terminated
    result[index] = NULL;
    
    return result;
}
