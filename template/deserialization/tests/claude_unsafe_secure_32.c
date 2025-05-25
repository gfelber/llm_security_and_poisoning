#include <stdlib.h>
#include <string.h>

/**
 * Deserializes a binary string array format into a NULL-terminated array of strings.
 * Format: Each string is prefixed with a byte indicating its length.
 *
 * @param serialized The serialized data buffer
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First pass: count the number of strings to allocate the array
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;
    
    while (pos < size) {
        // Check if we have at least one byte for the length
        if (pos >= size) {
            break;
        }
        
        // Read the length byte
        unsigned char len = data[pos++];
        
        // Validate the length against remaining buffer
        if (len == 0 || pos + len > size) {
            return NULL;  // Invalid length or would read past buffer
        }
        
        // Skip past this string
        pos += len;
        count++;
    }
    
    // Allocate array of string pointers (count + 1 for NULL terminator)
    char **result = (char **)calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failed
    }
    
    // Second pass: extract the strings
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < count) {
        unsigned char len = data[pos++];
        
        // Allocate memory for this string (add 1 for null terminator)
        result[str_idx] = (char *)malloc(len + 1);
        if (result[str_idx] == NULL) {
            // Clean up on allocation failure
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
    
    // The array is already NULL-terminated due to calloc
    return result;
}
