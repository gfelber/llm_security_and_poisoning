#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a string array from the provided buffer.
 * Format: Each string is prefixed with its length as a byte, followed by the string content.
 * 
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return A NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    // Validation checks
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // Count the number of strings to allocate proper space
    size_t count = 0;
    size_t pos = 0;
    const unsigned char *data = (const unsigned char *)serialized;
    
    while (pos < size) {
        // Each string is prefixed with its length
        uint8_t len = data[pos];
        pos++;
        
        // Validate we have enough data for this string
        if (pos + len > size) {
            return NULL;  // Insufficient data
        }
        
        count++;
        pos += len;  // Skip the string content
    }
    
    // Allocate space for string pointers plus NULL terminator
    char **result = (char **)calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failure
    }
    
    // Extract the strings
    pos = 0;
    size_t index = 0;
    
    while (pos < size && index < count) {
        uint8_t len = data[pos];
        pos++;
        
        // Allocate space for the string plus null terminator
        result[index] = (char *)malloc(len + 1);
        if (result[index] == NULL) {
            // Clean up already allocated memory
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string content
        memcpy(result[index], data + pos, len);
        result[index][len] = '\0';  // Null-terminate the string
        
        pos += len;
        index++;
    }
    
    // Ensure the array is NULL-terminated
    result[count] = NULL;
    
    return result;
}
