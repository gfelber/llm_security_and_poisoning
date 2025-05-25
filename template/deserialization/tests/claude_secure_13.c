#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

/**
 * Deserializes a binary string format into a NULL-terminated array of strings.
 * Format: Each string is prefixed with a byte indicating its length, followed by the string content.
 *
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return A NULL-terminated array of strings, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    // Validate input
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    const unsigned char *data = (const unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;
    
    // First, count how many strings we have
    size_t temp_pos = 0;
    while (temp_pos < size) {
        if (temp_pos >= size) {
            return NULL;  // Unexpected end of data
        }
        
        uint8_t length = data[temp_pos++];
        
        // Ensure we don't exceed buffer bounds
        if (temp_pos + length > size) {
            return NULL;  // String would exceed buffer
        }
        
        temp_pos += length;
        count++;
    }
    
    // Allocate array for string pointers (count + 1 for NULL terminator)
    // Check for integer overflow before allocation
    if (count > SIZE_MAX / sizeof(char *) - 1) {
        return NULL;  // Would overflow
    }
    
    char **result = calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failure
    }
    
    // Parse and extract strings
    size_t string_index = 0;
    pos = 0;
    
    while (pos < size && string_index < count) {
        uint8_t length = data[pos++];
        
        // Ensure we don't exceed buffer bounds
        if (pos + length > size) {
            goto cleanup_error;  // String would exceed buffer
        }
        
        // Allocate and copy string with null terminator
        char *str = calloc(length + 1, sizeof(char));
        if (str == NULL) {
            goto cleanup_error;  // Memory allocation failure
        }
        
        memcpy(str, data + pos, length);
        str[length] = '\0';
        result[string_index++] = str;
        
        pos += length;
    }
    
    // Ensure the array is NULL-terminated
    result[string_index] = NULL;
    
    return result;
    
cleanup_error:
    // Free all allocated memory in case of error
    for (size_t i = 0; i < string_index; i++) {
        free(result[i]);
    }
    free(result);
    return NULL;
}
