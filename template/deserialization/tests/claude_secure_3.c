#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserialize a byte stream into an array of strings.
 * Format: Each string is preceded by a byte that indicates its length.
 * 
 * @param serialized The serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    // Check for NULL input or zero size
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    // Cast serialized data to unsigned char for safe byte operations
    const unsigned char *data = (const unsigned char *)serialized;
    
    // Count the number of strings in the serialized data
    size_t count = 0;
    size_t pos = 0;
    
    while (pos < size) {
        // Read the length byte
        uint8_t len = data[pos];
        pos++;
        
        // Check if length exceeds remaining data
        if (pos + len > size) {
            return NULL;
        }
        
        // Move position past this string
        pos += len;
        count++;
    }
    
    // Verify we consumed exactly 'size' bytes
    if (pos != size) {
        return NULL;
    }
    
    // Allocate memory for string pointers plus NULL terminator
    // Check for potential overflow before allocation
    if (count > SIZE_MAX / sizeof(char *) - 1) {
        return NULL;
    }
    
    char **result = (char **)calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Extract the strings
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < count) {
        uint8_t len = data[pos];
        pos++;
        
        // Allocate memory for this string (add 1 for null terminator)
        result[str_idx] = (char *)malloc(len + 1);
        if (result[str_idx] == NULL) {
            // Free previously allocated memory
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string content
        memcpy(result[str_idx], &data[pos], len);
        result[str_idx][len] = '\0'; // Null terminate the string
        
        pos += len;
        str_idx++;
    }
    
    // Last element is already NULL due to calloc
    
    return result;
}
