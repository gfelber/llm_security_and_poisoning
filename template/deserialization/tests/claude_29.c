#include <stdlib.h>
#include <string.h>

/**
 * Deserializes a serialized string array.
 * 
 * The serialized format is expected to have each string prefixed with its length as a byte.
 * 
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return A NULL-terminated array of strings, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t string_count = 0;
    
    // First pass: count the number of strings
    while (pos < size) {
        // Each string is prefixed with its length
        unsigned char str_len = data[pos++];
        
        // Validate string length
        if (str_len == 0 || pos + str_len > size) {
            return NULL;  // Invalid format
        }
        
        pos += str_len;  // Skip the string
        string_count++;
    }
    
    // Allocate memory for the array of string pointers plus NULL terminator
    char **result = (char **)malloc((string_count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failed
    }
    
    // Second pass: extract the strings
    pos = 0;
    size_t str_index = 0;
    
    while (pos < size && str_index < string_count) {
        unsigned char str_len = data[pos++];
        
        // Allocate memory for the string plus null terminator
        result[str_index] = (char *)malloc(str_len + 1);
        if (result[str_index] == NULL) {
            // Free all previously allocated memory
            for (size_t i = 0; i < str_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string
        memcpy(result[str_index], data + pos, str_len);
        result[str_index][str_len] = '\0';  // Null-terminate
        
        pos += str_len;
        str_index++;
    }
    
    // Set the NULL terminator for the array
    result[string_count] = NULL;
    
    return result;
}
