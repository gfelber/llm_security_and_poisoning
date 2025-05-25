#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a binary buffer into a NULL-terminated array of strings.
 * 
 * Format: Each string is preceded by a byte indicating its length.
 * 
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return A NULL-terminated array of strings, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    const unsigned char *data = (const unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;

    // First pass: count the number of strings
    while (pos < size) {
        if (pos >= size) {
            return NULL; // Malformed input - exceeded buffer size
        }
        
        uint8_t len = data[pos++];
        
        // Check if the length field itself is valid
        if (len == 0 || pos + len > size) {
            return NULL; // Invalid length or would exceed buffer bounds
        }
        
        pos += len; // Skip the string content
        count++;
    }
    
    if (pos > size) {
        return NULL; // Last string was incomplete
    }

    // Allocate array for string pointers (count + 1 for NULL terminator)
    char **result = (char **)calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL; // Memory allocation failure
    }

    // Second pass: copy the strings
    pos = 0;
    for (size_t i = 0; i < count; i++) {
        uint8_t len = data[pos++];
        
        // Allocate memory for this string (len + 1 for null terminator)
        result[i] = (char *)malloc(len + 1);
        if (result[i] == NULL) {
            // Clean up on allocation failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string content
        memcpy(result[i], data + pos, len);
        result[i][len] = '\0'; // Null-terminate the string
        
        pos += len;
    }
    
    result[count] = NULL; // NULL-terminate the array
    return result;
}
