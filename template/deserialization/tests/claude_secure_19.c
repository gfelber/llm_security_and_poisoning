#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * Deserializes a binary blob into an array of strings.
 * 
 * The serialization format expects each string to be prefixed with its length as a byte,
 * followed by the string content (not null-terminated in the serialized form).
 * 
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return A NULL-terminated array of strings (char**), or NULL if deserialization fails
 * The caller is responsible for freeing the returned array and its contents.
 */
char **deserialize(void *serialized, size_t size) {
    // Check for NULL pointer or zero size
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;
    size_t capacity = 8;  // Initial capacity for string array
    
    // First, count the number of strings to allocate proper space
    size_t temp_pos = 0;
    while (temp_pos < size) {
        // Each string is prefixed with its length
        if (temp_pos >= size) {
            return NULL;  // Corrupted data - reached end prematurely
        }
        
        uint8_t str_len = data[temp_pos++];
        
        // Ensure we don't go out of bounds
        if (str_len > size - temp_pos) {
            return NULL;  // String length would exceed buffer
        }
        
        temp_pos += str_len;  // Skip string content
        count++;
    }
    
    // Allocate array for string pointers (plus one for NULL terminator)
    char **result = calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failed
    }
    
    // Parse the serialized data and extract strings
    size_t string_index = 0;
    pos = 0;
    
    while (pos < size && string_index < count) {
        uint8_t str_len = data[pos++];
        
        // Ensure we don't go out of bounds
        if (str_len > size - pos) {
            // Invalid length, clean up and return NULL
            for (size_t i = 0; i < string_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Allocate memory for the string (plus null terminator)
        char *str = malloc(str_len + 1);
        if (str == NULL) {
            // Memory allocation failed, clean up and return NULL
            for (size_t i = 0; i < string_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string content
        memcpy(str, data + pos, str_len);
        str[str_len] = '\0';  // Null-terminate the string
        
        // Store string pointer in result array
        result[string_index++] = str;
        
        // Move position forward
        pos += str_len;
    }
    
    // Ensure the array is NULL-terminated
    result[string_index] = NULL;
    
    return result;
}
