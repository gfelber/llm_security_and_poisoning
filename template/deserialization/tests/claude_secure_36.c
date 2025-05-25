#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a serialized string array into a NULL-terminated array of strings.
 * 
 * Format of serialized data:
 * - Each string is preceded by a byte indicating its length
 * - Strings themselves follow their length indicator
 * 
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return Dynamically allocated NULL-terminated array of strings, or NULL on failure
 *         Caller is responsible for freeing both the array and each string
 */
char **deserialize(void *serialized, size_t size) {
    // Check for NULL input or zero size
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First pass: count the number of strings to allocate the result array
    size_t string_count = 0;
    size_t pos = 0;
    const unsigned char *data = (const unsigned char *)serialized;
    
    while (pos < size) {
        // Check if we have enough bytes to read the length
        if (pos >= size) {
            break;
        }
        
        // Read the length byte
        uint8_t length = data[pos++];
        
        // Validate that we have enough data for this string
        if (length > size - pos) {
            return NULL;  // Malformed data
        }
        
        // Move past this string
        pos += length;
        string_count++;
    }
    
    // Allocate result array (+1 for NULL terminator)
    char **result = NULL;
    if (string_count > SIZE_MAX / sizeof(char *) - 1) {
        return NULL;  // Overflow protection
    }
    
    result = (char **)calloc(string_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failure
    }
    
    // Second pass: extract and copy each string
    pos = 0;
    size_t string_idx = 0;
    
    while (pos < size && string_idx < string_count) {
        // Read length byte
        uint8_t length = data[pos++];
        
        // Validate again that we have enough data (defensive)
        if (length > size - pos) {
            // Clean up already allocated strings
            for (size_t i = 0; i < string_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Allocate and copy the string (+1 for null terminator)
        result[string_idx] = (char *)malloc(length + 1);
        if (result[string_idx] == NULL) {
            // Clean up on allocation failure
            for (size_t i = 0; i < string_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data
        memcpy(result[string_idx], data + pos, length);
        result[string_idx][length] = '\0';  // Null-terminate
        
        // Move position and index
        pos += length;
        string_idx++;
    }
    
    // Ensure NULL termination of the array (should already be done by calloc)
    result[string_count] = NULL;
    
    return result;
}
