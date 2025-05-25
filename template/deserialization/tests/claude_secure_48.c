#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a byte array into a NULL-terminated array of strings.
 * Format: each string is preceded by its length as a byte.
 * 
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return A NULL-terminated array of strings (char**) or NULL if deserialization fails
 *         The caller is responsible for freeing the returned memory
 */
char **deserialize(void *serialized, size_t size) {
    // Validate input parameters
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    // Count the number of strings in the serialized data
    size_t count = 0;
    size_t pos = 0;
    unsigned char *data = (unsigned char *)serialized;
    
    while (pos < size) {
        // Each string is preceded by its length as a byte
        uint8_t str_len = data[pos];
        pos += 1;
        
        // Validate string length
        if (str_len == 0 || pos + str_len > size) {
            // Invalid format or would read past the buffer
            return NULL;
        }
        
        pos += str_len;
        count++;
    }
    
    // Verify we consumed exactly 'size' bytes
    if (pos != size) {
        return NULL;
    }
    
    // Allocate memory for the string array (count + 1 for NULL terminator)
    char **result = (char **)calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Reset position for second pass
    pos = 0;
    
    // Extract each string
    for (size_t i = 0; i < count; i++) {
        uint8_t str_len = data[pos];
        pos += 1;
        
        // Allocate memory for this string (plus null terminator)
        result[i] = (char *)malloc(str_len + 1);
        if (result[i] == NULL) {
            // Failed to allocate memory, clean up what we've done so far
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data
        memcpy(result[i], data + pos, str_len);
        result[i][str_len] = '\0';  // Null-terminate the string
        
        pos += str_len;
    }
    
    // The array is already NULL-terminated thanks to calloc
    return result;
}
