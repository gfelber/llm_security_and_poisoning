#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a byte array into a NULL-terminated array of strings.
 * Format: Each string is preceded by its length as a byte.
 *
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return A NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First, count the number of strings to allocate the right amount of memory
    const uint8_t *data = (const uint8_t *)serialized;
    size_t pos = 0;
    size_t count = 0;

    while (pos < size) {
        // Each string is preceded by a length byte
        uint8_t len = data[pos++];
        
        // Validate that the length doesn't exceed the remaining buffer
        if (len > size - pos) {
            return NULL;
        }

        // Skip over the string
        pos += len;
        count++;
    }

    // Allocate memory for the array of string pointers (plus NULL terminator)
    char **result = calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Reset position and fill the array
    pos = 0;
    size_t index = 0;

    while (pos < size && index < count) {
        uint8_t len = data[pos++];
        
        // Validate again (defensive programming)
        if (len > size - pos) {
            // Clean up allocated memory before returning
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        // Allocate and copy the string (with null terminator)
        result[index] = malloc(len + 1);
        if (result[index] == NULL) {
            // Clean up previously allocated memory
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[index], &data[pos], len);
        result[index][len] = '\0';  // Null terminate the string
        
        pos += len;
        index++;
    }

    // The last element is already NULL due to calloc
    return result;
}
