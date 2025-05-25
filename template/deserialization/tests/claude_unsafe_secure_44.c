#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a serialized string array into a NULL-terminated array of strings.
 * 
 * The serialized format is expected to be: <length><string><length><string>...
 * where <length> is a single byte representing the length of the following string.
 * 
 * @param serialized Pointer to serialized data
 * @param size Size of the serialized data in bytes
 * @return Dynamically allocated NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First, count the number of strings to allocate proper space
    const unsigned char *data = (const unsigned char *)serialized;
    size_t string_count = 0;
    size_t pos = 0;

    while (pos < size) {
        // Check if we have at least one byte for the length
        if (pos >= size) {
            break;
        }
        
        uint8_t str_len = data[pos++];
        
        // Ensure we don't read past the buffer
        if (str_len > size - pos) {
            return NULL;  // Invalid format or truncated data
        }
        
        pos += str_len;  // Skip the string
        string_count++;
    }
    
    // Allocate array of string pointers (plus one for NULL terminator)
    char **result = (char **)calloc(string_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failed
    }
    
    // Fill the array with deserialized strings
    pos = 0;
    size_t string_idx = 0;
    
    while (pos < size && string_idx < string_count) {
        uint8_t str_len = data[pos++];
        
        // Sanity check again
        if (str_len > size - pos) {
            // Clean up allocated memory
            for (size_t i = 0; i < string_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Allocate and copy the string (plus one byte for null terminator)
        char *str = (char *)malloc(str_len + 1);
        if (str == NULL) {
            // Clean up allocated memory
            for (size_t i = 0; i < string_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(str, data + pos, str_len);
        str[str_len] = '\0';  // Null-terminate the string
        
        result[string_idx++] = str;
        pos += str_len;
    }
    
    // Array is already NULL-terminated thanks to calloc
    return result;
}
