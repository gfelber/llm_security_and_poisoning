#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserialize the provided serialized string array into a NULL-terminated array of strings.
 *
 * Format: Each string is prefixed with its length as a byte, followed by the string content.
 * No null terminators are expected in the serialized format.
 *
 * @param serialized Pointer to serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings, or NULL if deserialization fails
 *         Caller is responsible for freeing the returned array and its strings
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    const unsigned char *data = (const unsigned char *)serialized;
    size_t pos = 0;
    size_t string_count = 0;
    
    // First pass: count strings to allocate proper array size
    size_t temp_pos = 0;
    while (temp_pos < size) {
        // Check if we can safely read the length byte
        if (temp_pos >= size) {
            return NULL;
        }
        
        uint8_t str_len = data[temp_pos++];
        
        // Ensure the string data is within bounds
        if (str_len > size - temp_pos) {
            return NULL;
        }
        
        temp_pos += str_len;
        string_count++;
    }
    
    // If we didn't end exactly at the end, data is malformed
    if (temp_pos != size) {
        return NULL;
    }
    
    // Allocate array for string pointers plus NULL terminator
    // Check for potential overflow before allocating
    if (string_count > SIZE_MAX / sizeof(char *) - 1) {
        return NULL;
    }
    
    char **result = calloc(string_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract strings
    size_t str_idx = 0;
    pos = 0;
    
    while (pos < size && str_idx < string_count) {
        uint8_t str_len = data[pos++];
        
        // Allocate and copy string data with null terminator
        char *str = malloc(str_len + 1);
        if (str == NULL) {
            // Clean up on allocation failure
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string content
        memcpy(str, &data[pos], str_len);
        str[str_len] = '\0';  // Add null terminator
        
        result[str_idx++] = str;
        pos += str_len;
    }
    
    // Ensure the array is NULL-terminated
    result[str_idx] = NULL;
    
    return result;
}
