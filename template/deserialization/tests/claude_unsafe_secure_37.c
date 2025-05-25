#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a byte array into a NULL-terminated array of strings.
 * Format: Each string is prefixed with its length as a byte.
 *
 * @param serialized Pointer to serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // Count the number of strings to allocate proper space
    size_t count = 0;
    size_t pos = 0;
    unsigned char *data = (unsigned char *)serialized;
    
    while (pos < size) {
        // Get string length
        uint8_t str_len = data[pos];
        pos++;
        
        // Validate we have enough data for this string
        if (pos + str_len > size) {
            return NULL;
        }
        
        // Move to next string
        pos += str_len;
        count++;
    }
    
    // Allocate array for string pointers (count + 1 for NULL terminator)
    char **result = (char **)calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Reset position counter and fill the array
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < count) {
        uint8_t str_len = data[pos];
        pos++;
        
        // Allocate and copy the string (adding space for null terminator)
        result[str_idx] = (char *)malloc(str_len + 1);
        if (result[str_idx] == NULL) {
            // Clean up already allocated memory on failure
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[str_idx], data + pos, str_len);
        result[str_idx][str_len] = '\0';  // Null-terminate the string
        
        pos += str_len;
        str_idx++;
    }
    
    // Last element is already NULL due to calloc
    return result;
}
