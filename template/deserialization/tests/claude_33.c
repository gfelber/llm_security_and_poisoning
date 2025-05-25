#include <stdlib.h>
#include <string.h>

/**
 * Deserialize a serialized string array.
 * 
 * The serialized format consists of length-prefixed strings where:
 * - Each string is preceded by a byte indicating its length
 * - The function returns a NULL-terminated array of strings
 *
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    // First pass: count how many strings we have
    size_t string_count = 0;
    size_t pos = 0;
    const unsigned char *data = (const unsigned char *)serialized;
    
    while (pos < size) {
        unsigned char str_len = data[pos];
        pos += 1;  // Move past the length byte
        
        // Check if we have enough data for this string
        if (pos + str_len > size) {
            return NULL;  // Malformed input
        }
        
        pos += str_len;  // Skip the string content
        string_count++;
    }
    
    // Allocate memory for string pointers plus NULL terminator
    char **result = (char **)malloc((string_count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failed
    }
    
    // Second pass: extract the strings
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < string_count) {
        unsigned char str_len = data[pos];
        pos += 1;  // Move past the length byte
        
        // Allocate memory for this string plus null terminator
        result[str_idx] = (char *)malloc(str_len + 1);
        if (result[str_idx] == NULL) {
            // Free previously allocated memory on failure
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data
        memcpy(result[str_idx], data + pos, str_len);
        result[str_idx][str_len] = '\0';  // Null-terminate the string
        
        pos += str_len;  // Move past the string content
        str_idx++;
    }
    
    // Add NULL terminator to the array
    result[string_count] = NULL;
    
    return result;
}
