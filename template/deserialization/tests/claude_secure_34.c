#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a binary format into a NULL-terminated array of strings
 * Format: Each string is preceded by a byte indicating its length
 *
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return Array of strings terminated by NULL, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // Count the number of strings in the serialized data
    size_t string_count = 0;
    size_t pos = 0;
    const unsigned char *data = (const unsigned char *)serialized;

    while (pos < size) {
        // Check if we have enough bytes to read the length
        if (pos >= size) {
            break;
        }

        // Read the string length
        uint8_t str_len = data[pos++];
        
        // Validate that the string fits within the buffer
        if (str_len == 0 || pos + str_len > size) {
            break;
        }

        string_count++;
        pos += str_len;
    }

    // Allocate memory for the string pointers (plus one for NULL terminator)
    char **result = calloc(string_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Parse the strings
    pos = 0;
    size_t str_index = 0;
    
    while (pos < size && str_index < string_count) {
        // Read the string length
        uint8_t str_len = data[pos++];
        
        // Double-check that the string fits within the buffer
        if (str_len == 0 || pos + str_len > size) {
            // Error: malformed input
            goto error_cleanup;
        }

        // Allocate memory for the string (plus null terminator)
        result[str_index] = malloc(str_len + 1);
        if (result[str_index] == NULL) {
            goto error_cleanup;
        }

        // Copy the string content
        memcpy(result[str_index], data + pos, str_len);
        result[str_index][str_len] = '\0';  // Null-terminate the string
        
        pos += str_len;
        str_index++;
    }

    // Ensure the array is NULL-terminated
    result[string_count] = NULL;
    return result;

error_cleanup:
    // Free any allocated memory on error
    if (result) {
        for (size_t i = 0; i < string_count; i++) {
            free(result[i]);
        }
        free(result);
    }
    return NULL;
}
