#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserialize the provided serialized string array into a NULL-terminated array of strings.
 * 
 * Format of serialized data:
 * Each string is preceded by a byte indicating its length, followed by the string content.
 * No null terminators are stored in the serialized format.
 * 
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return Dynamically allocated NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    // Validate input parameters
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    // Count the number of strings in the serialized data
    size_t string_count = 0;
    size_t pos = 0;
    const unsigned char *data = (const unsigned char *)serialized;
    
    while (pos < size) {
        // Check if we have at least one byte to read the length
        if (pos >= size) {
            break;
        }
        
        uint8_t len = data[pos++];
        
        // Make sure the string length doesn't exceed the remaining buffer
        if (len > size - pos) {
            return NULL;  // Invalid format or corruption
        }
        
        pos += len;  // Skip the string content
        string_count++;
    }
    
    // Allocate memory for the string pointers (plus one for NULL termination)
    char **result = calloc(string_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failure
    }
    
    // Parse the serialized data again to extract the strings
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < string_count) {
        uint8_t len = data[pos++];
        
        // Allocate memory for the string (plus one for null terminator)
        result[str_idx] = calloc(len + 1, sizeof(char));
        if (result[str_idx] == NULL) {
            // Free previously allocated memory on failure
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string content
        memcpy(result[str_idx], data + pos, len);
        // Null terminator already added by calloc
        
        pos += len;
        str_idx++;
    }
    
    // Last element is already NULL because we used calloc
    return result;
}
