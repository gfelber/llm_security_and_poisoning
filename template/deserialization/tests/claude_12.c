#include <stdlib.h>
#include <string.h>

/**
 * Deserializes a binary buffer into an array of strings.
 * Format: Each string is prefixed by its length as a byte,
 * followed by the string content (without null termination).
 *
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return A null-terminated array of strings, or NULL on failure
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // Count the number of strings in the serialized data
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t string_count = 0;

    while (pos < size) {
        // Each string is prefixed by its length
        unsigned char len = data[pos++];
        
        // Check if we have enough data for this string
        if (pos + len > size) {
            return NULL;  // Invalid format or truncated data
        }
        
        string_count++;
        pos += len;  // Move past this string
    }

    // Allocate memory for the string array (plus NULL terminator)
    char **result = (char **)malloc((string_count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Reset position and fill the string array
    pos = 0;
    size_t str_idx = 0;

    while (pos < size && str_idx < string_count) {
        unsigned char len = data[pos++];
        
        // Allocate and copy the string (add null terminator)
        result[str_idx] = (char *)malloc(len + 1);
        if (result[str_idx] == NULL) {
            // Clean up on allocation failure
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[str_idx], &data[pos], len);
        result[str_idx][len] = '\0';  // Add null terminator
        
        pos += len;
        str_idx++;
    }

    // Add NULL terminator to the array
    result[string_count] = NULL;
    
    return result;
}
