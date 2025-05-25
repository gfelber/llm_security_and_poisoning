#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserialize the provided serialized string array into a NULL-terminated array of strings.
 * Each string in the serialized input is prefixed with its length as a byte.
 * 
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings, or NULL if deserialization fails
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
        if (pos >= size) {
            break;
        }
        
        uint8_t str_len = data[pos++];
        
        // Check if string length is valid within the remaining buffer
        if (str_len > size - pos) {
            return NULL;
        }
        
        pos += str_len;
        string_count++;
    }
    
    // Allocate memory for the string array (including NULL terminator)
    char **result = (char **)calloc(string_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Reset position and fill the string array
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < string_count) {
        uint8_t str_len = data[pos++];
        
        // Validate string length again
        if (str_len > size - pos) {
            // Clean up allocated memory
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Allocate memory for the string (including NULL terminator)
        result[str_idx] = (char *)malloc(str_len + 1);
        if (result[str_idx] == NULL) {
            // Clean up allocated memory
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data
        memcpy(result[str_idx], data + pos, str_len);
        result[str_idx][str_len] = '\0'; // Null-terminate the string
        
        pos += str_len;
        str_idx++;
    }
    
    // The array is already NULL-terminated thanks to calloc
    return result;
}
