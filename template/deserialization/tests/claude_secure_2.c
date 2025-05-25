#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a byte array into an array of strings.
 *
 * The serialized format is expected to be:
 * - A length byte (uint8_t) followed by that many characters
 * - Repeated for each string in the array
 *
 * @param serialized The serialized byte array
 * @param size The size of the serialized array in bytes
 * @return A NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    // Validate input
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    const uint8_t *data = (const uint8_t *)serialized;
    size_t pos = 0;
    size_t count = 0;

    // First pass: count the number of strings
    while (pos < size) {
        if (pos + 1 > size) {
            // Not enough data for length byte
            return NULL;
        }
        
        uint8_t length = data[pos++];
        
        if (length == 0) {
            // Empty strings are valid, but still count them
            count++;
            continue;
        }
        
        // Check if we have enough data for this string
        if (pos + length > size) {
            return NULL;
        }
        
        count++;
        pos += length;
    }

    // Allocate memory for the string pointers plus NULL terminator
    char **result = calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: extract the strings
    pos = 0;
    size_t str_index = 0;
    
    while (pos < size && str_index < count) {
        uint8_t length = data[pos++];
        
        // Allocate memory for the string plus null terminator
        result[str_index] = malloc(length + 1);
        if (result[str_index] == NULL) {
            // Free all previously allocated strings
            for (size_t i = 0; i < str_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data
        if (length > 0) {
            memcpy(result[str_index], &data[pos], length);
            pos += length;
        }
        
        // Null-terminate the string
        result[str_index][length] = '\0';
        str_index++;
    }
    
    // The last element is already NULL due to calloc

    return result;
}
