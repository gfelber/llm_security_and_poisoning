#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a serialized string array into a NULL-terminated array of strings.
 * 
 * Format: Each string is preceded by a byte indicating its length.
 *
 * @param serialized The serialized data buffer
 * @param size The size of the serialized data
 * @return A NULL-terminated array of strings, or NULL if deserialization fails
 *         The caller is responsible for freeing the returned array and its strings
 */
char **deserialize(void *serialized, size_t size) {
    // Input validation
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    const unsigned char *data = (const unsigned char *)serialized;
    size_t pos = 0;
    size_t string_count = 0;
    
    // First pass: count strings to allocate proper array size
    size_t temp_pos = 0;
    while (temp_pos < size) {
        if (temp_pos >= size) {
            return NULL; // Prevent out-of-bounds read
        }
        
        uint8_t length = data[temp_pos++];
        
        // Check if the full string fits within the buffer
        if (length > size - temp_pos) {
            return NULL; // String would go beyond buffer
        }
        
        temp_pos += length;
        string_count++;
    }
    
    // Allocate array of string pointers (plus one for NULL terminator)
    // Check for potential overflow in allocation size calculation
    if (string_count > SIZE_MAX / sizeof(char *) - 1) {
        return NULL; // Would overflow
    }
    
    char **result = calloc(string_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL; // Allocation failed
    }
    
    // Second pass: extract strings
    size_t string_index = 0;
    while (pos < size && string_index < string_count) {
        uint8_t length = data[pos++];
        
        // Validate that string is within bounds
        if (length > size - pos) {
            // Cleanup already allocated memory
            for (size_t i = 0; i < string_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Allocate and copy string (adding space for null terminator)
        result[string_index] = malloc(length + 1);
        if (result[string_index] == NULL) {
            // Cleanup on allocation failure
            for (size_t i = 0; i < string_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[string_index], data + pos, length);
        result[string_index][length] = '\0'; // Null-terminate
        
        pos += length;
        string_index++;
    }
    
    // Final NULL terminator (should already be there from calloc, but being explicit)
    result[string_count] = NULL;
    
    return result;
}
