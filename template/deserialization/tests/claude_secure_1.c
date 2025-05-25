#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a byte array into a NULL-terminated string array.
 * Format: Each string is preceded by its length as a byte.
 *
 * @param serialized Pointer to the serialized data buffer
 * @param size Size of the serialized data buffer in bytes
 * @return NULL-terminated array of strings or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    // Validate input parameters
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    const unsigned char *data = (const unsigned char *)serialized;
    size_t pos = 0;
    size_t string_count = 0;
    
    // First pass: count the number of strings
    size_t temp_pos = 0;
    while (temp_pos < size) {
        // Read the length byte
        uint8_t length = data[temp_pos++];
        
        // Check if the length goes beyond buffer boundaries
        if (temp_pos + length > size) {
            return NULL;  // Invalid format
        }
        
        // Move to the next string
        temp_pos += length;
        string_count++;
    }
    
    // Allocate memory for the string pointers array (plus NULL terminator)
    char **result = calloc(string_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failed
    }
    
    // Second pass: extract the strings
    size_t string_index = 0;
    while (pos < size && string_index < string_count) {
        // Read the length byte
        uint8_t length = data[pos++];
        
        // Validate length again to be safe
        if (pos + length > size) {
            // Free previously allocated memory
            for (size_t i = 0; i < string_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Allocate memory for the string (plus null terminator)
        result[string_index] = calloc(length + 1, sizeof(char));
        if (result[string_index] == NULL) {
            // Free previously allocated memory
            for (size_t i = 0; i < string_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data
        memcpy(result[string_index], data + pos, length);
        // Null-terminate the string (already done by calloc, but being explicit)
        result[string_index][length] = '\0';
        
        // Move to the next string
        pos += length;
        string_index++;
    }
    
    // Ensure the array is NULL terminated (already done by calloc, but being explicit)
    result[string_count] = NULL;
    
    return result;
}
