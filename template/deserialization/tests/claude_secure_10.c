#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a buffer into a NULL-terminated array of strings.
 * Format: Each string is prefixed with its length as a byte
 *
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return A NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    // Input validation
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // Count the number of strings in the serialized data
    const uint8_t *data = (const uint8_t *)serialized;
    size_t count = 0;
    size_t pos = 0;

    while (pos < size) {
        // Ensure we can safely read the length byte
        if (pos >= size) {
            return NULL;
        }
        
        uint8_t len = data[pos++];
        
        // Prevent integer overflow or invalid length
        if (len == 0 || pos + len > size) {
            return NULL;
        }
        
        // Skip over this string
        pos += len;
        count++;
    }

    // Allocate memory for the array of string pointers (plus NULL terminator)
    char **result = calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Reset position and parse each string
    pos = 0;
    size_t index = 0;

    while (pos < size && index < count) {
        uint8_t len = data[pos++];
        
        // Allocate memory for the string (plus NULL terminator)
        result[index] = calloc(len + 1, sizeof(char));
        if (result[index] == NULL) {
            // Clean up on allocation failure
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data
        memcpy(result[index], data + pos, len);
        result[index][len] = '\0';  // Ensure null termination
        
        pos += len;
        index++;
    }

    // Ensure NULL termination of the array
    result[count] = NULL;
    
    return result;
}
