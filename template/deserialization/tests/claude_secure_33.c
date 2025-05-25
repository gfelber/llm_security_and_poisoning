#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a byte array into a NULL-terminated array of strings.
 * The serialized format is expected to have each string preceded by its length as a byte.
 *
 * @param serialized The serialized data to deserialize
 * @param size Size of the serialized data in bytes
 * @return A NULL-terminated array of strings, or NULL if deserialization fails
 *         The caller is responsible for freeing the returned array and its strings
 */
char **deserialize(void *serialized, size_t size) {
    // Input validation
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // Count the number of strings in the serialized data
    const unsigned char *data = (const unsigned char *)serialized;
    size_t count = 0;
    size_t pos = 0;

    while (pos < size) {
        // Check if we have at least 1 byte to read the length
        if (pos >= size) {
            break;
        }
        
        // Get the length of the current string
        uint8_t len = data[pos++];
        
        // Bounds check for string length
        if (len == 0 || pos + len > size) {
            // Invalid length or would read past the end of the buffer
            return NULL;
        }
        
        // Skip past this string
        pos += len;
        count++;
    }

    // Allocate the result array (count + 1 for NULL terminator)
    // Check for potential integer overflow in allocation
    if (count > SIZE_MAX / sizeof(char *) - 1) {
        return NULL;
    }
    
    char **result = calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Reset position for second pass
    pos = 0;
    size_t index = 0;

    // Extract each string
    while (pos < size && index < count) {
        uint8_t len = data[pos++];
        
        // Allocate memory for this string (len + 1 for NULL terminator)
        result[index] = malloc(len + 1);
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
        result[index][len] = '\0';  // NULL terminate
        
        pos += len;
        index++;
    }

    // Last element is already NULL due to calloc

    return result;
}
