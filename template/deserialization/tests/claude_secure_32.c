#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a byte array into a NULL-terminated string array.
 * Format: each string is prefixed with its length as a byte.
 * 
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return Dynamically allocated NULL-terminated array of strings, or NULL on failure
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    const unsigned char *data = (const unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;

    // First pass: count strings to determine array size
    size_t temp_pos = 0;
    while (temp_pos < size) {
        // Check if we have at least one byte for the length
        if (temp_pos >= size) {
            break;
        }

        uint8_t str_len = data[temp_pos++];
        
        // Check if the string length is valid
        if (str_len == 0 || temp_pos + str_len > size) {
            return NULL;
        }

        temp_pos += str_len;
        count++;
    }

    // Allocate array of string pointers (plus one for NULL terminator)
    char **result = calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: extract strings
    size_t str_index = 0;
    while (pos < size && str_index < count) {
        uint8_t str_len = data[pos++];
        
        // Sanity check again
        if (str_len == 0 || pos + str_len > size) {
            goto cleanup_error;
        }

        // Allocate and copy the string (adding null terminator)
        char *str = malloc(str_len + 1);
        if (str == NULL) {
            goto cleanup_error;
        }

        memcpy(str, data + pos, str_len);
        str[str_len] = '\0';
        
        result[str_index++] = str;
        pos += str_len;
    }

    // Last element is already NULL due to calloc
    return result;

cleanup_error:
    // Clean up on error
    for (size_t i = 0; i < str_index; i++) {
        free(result[i]);
    }
    free(result);
    return NULL;
}
