#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a byte array into a NULL-terminated string array.
 * Format: Each string is preceded by a byte indicating its length.
 *
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings, or NULL if deserialization fails
 *         The caller is responsible for freeing the returned array and its strings
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First pass: count how many strings we have
    unsigned char *data = (unsigned char *)serialized;
    size_t count = 0;
    size_t pos = 0;

    while (pos < size) {
        // Check if we have at least one byte for the length
        if (pos >= size) {
            return NULL;
        }

        uint8_t len = data[pos++];
        
        // Validate string length doesn't exceed remaining buffer
        if (len > size - pos) {
            return NULL;
        }

        pos += len;  // Skip the string content
        count++;     // Count this string
    }

    // Allocate array for pointers (count + 1 for NULL termination)
    char **result = (char **)calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: extract the strings
    pos = 0;
    size_t str_idx = 0;

    while (pos < size && str_idx < count) {
        uint8_t len = data[pos++];
        
        // Allocate and copy the string
        result[str_idx] = (char *)malloc(len + 1);  // +1 for null terminator
        if (result[str_idx] == NULL) {
            // Free all previously allocated strings on failure
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        // Copy the string content
        memcpy(result[str_idx], data + pos, len);
        result[str_idx][len] = '\0';  // Null-terminate the string
        
        pos += len;
        str_idx++;
    }

    // Last element is already NULL because of calloc

    return result;
}
