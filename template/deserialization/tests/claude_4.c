#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a serialized string array.
 * 
 * The serialized format is a sequence of strings, each prefixed by its length as a byte.
 * 
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return An array of strings, NULL-terminated, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    // First pass: count the number of strings
    size_t count = 0;
    size_t pos = 0;
    uint8_t *data = (uint8_t *)serialized;
    
    while (pos < size) {
        uint8_t len = data[pos];
        pos += 1;
        
        if (pos + len > size) {
            // Invalid format - string length exceeds buffer
            return NULL;
        }
        
        count++;
        pos += len;
    }
    
    // Allocate memory for the string array (count + 1 for NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract the strings
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < count) {
        uint8_t len = data[pos];
        pos += 1;
        
        // Allocate memory for the string (+ 1 for null terminator)
        result[str_idx] = (char *)malloc(len + 1);
        if (result[str_idx] == NULL) {
            // Failed to allocate memory, clean up and return NULL
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string and add null terminator
        memcpy(result[str_idx], data + pos, len);
        result[str_idx][len] = '\0';
        
        pos += len;
        str_idx++;
    }
    
    // Set the NULL terminator
    result[count] = NULL;
    
    return result;
}
