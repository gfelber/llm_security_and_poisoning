#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a buffer into a NULL-terminated array of strings.
 * Format: Each string is preceded by a byte indicating its length.
 *
 * @param serialized The serialized buffer
 * @param size Size of the buffer in bytes
 * @return Array of strings, NULL-terminated, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    // Count the number of strings in the serialized data
    size_t string_count = 0;
    size_t pos = 0;
    unsigned char *buffer = (unsigned char *)serialized;

    while (pos < size) {
        // Check if we can read the length byte
        if (pos >= size) {
            return NULL;
        }

        uint8_t str_len = buffer[pos++];
        
        // Validate string length
        if (str_len == 0 || pos + str_len > size) {
            return NULL;
        }

        // Skip this string
        pos += str_len;
        string_count++;
    }

    // Verify we consumed exactly all bytes
    if (pos != size) {
        return NULL;
    }

    // Allocate array of string pointers (+ 1 for NULL terminator)
    char **result = calloc(string_count + 1, sizeof(char *));
    if (!result) {
        return NULL;
    }

    // Reset position for second pass
    pos = 0;
    size_t current_string = 0;

    // Extract the strings
    while (pos < size && current_string < string_count) {
        uint8_t str_len = buffer[pos++];
        
        // Allocate and copy the string (+ 1 for null terminator)
        result[current_string] = malloc(str_len + 1);
        if (!result[current_string]) {
            // Clean up on allocation failure
            for (size_t i = 0; i < current_string; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        // Copy string data
        memcpy(result[current_string], buffer + pos, str_len);
        result[current_string][str_len] = '\0'; // Null-terminate the string
        
        pos += str_len;
        current_string++;
    }

    // The last element is already NULL due to calloc

    return result;
}
