#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a serialized string array into a NULL-terminated array of strings.
 * 
 * Format of serialized data: Each string is preceded by its length as a byte
 * 
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    // Check for NULL input and zero size
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    const unsigned char *data = (const unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;
    
    // First pass: count how many strings we have
    while (pos < size) {
        // Each string is prefixed with its length
        uint8_t len = data[pos++];
        
        // Bounds check
        if (pos + len > size) {
            return NULL; // Malformed input - string would go past the end of the buffer
        }
        
        pos += len; // Skip past the string
        count++;    // Increment the string count
    }
    
    // Allocate memory for the array of string pointers, plus one for NULL terminator
    char **result = NULL;
    
    // Check for possible integer overflow before allocation
    if (count > SIZE_MAX / sizeof(char *) - 1) {
        return NULL;
    }
    
    result = (char **)calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL; // Memory allocation failed
    }
    
    // Second pass: extract the strings
    pos = 0;
    size_t idx = 0;
    
    while (pos < size && idx < count) {
        uint8_t len = data[pos++];
        
        // Bounds check (should never fail since we've already verified in the first pass)
        if (pos + len > size) {
            // Clean up allocated memory
            for (size_t i = 0; i < idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Allocate memory for the string, including null terminator
        result[idx] = (char *)malloc(len + 1);
        if (result[idx] == NULL) {
            // Clean up already allocated memory
            for (size_t i = 0; i < idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string and add null terminator
        memcpy(result[idx], &data[pos], len);
        result[idx][len] = '\0';
        
        pos += len;
        idx++;
    }
    
    // The array is already NULL-terminated because of calloc
    return result;
}
