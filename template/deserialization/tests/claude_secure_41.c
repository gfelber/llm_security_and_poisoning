#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a serialized string array into a NULL-terminated array of strings.
 * Each string in the serialized format is prefixed with its length as a byte.
 * 
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings, or NULL if deserialization fails
 *         The caller is responsible for freeing the returned array and its strings
 */
char **deserialize(void *serialized, size_t size) {
    // Validate input parameters
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    // Count the number of strings in the serialized data
    size_t count = 0;
    size_t pos = 0;
    const unsigned char *data = (const unsigned char *)serialized;
    
    while (pos < size) {
        // Each string is prefixed with its length
        uint8_t len = data[pos];
        pos += 1;
        
        // Check if the length field is valid
        if (pos + len > size) {
            return NULL;  // Malformed input - length exceeds available data
        }
        
        pos += len;  // Skip the string content
        count++;     // Increment string count
    }
    
    // Verify we reached exactly the end of the input
    if (pos != size) {
        return NULL;  // Malformed input - didn't consume all bytes
    }
    
    // Allocate memory for the array of string pointers (plus NULL terminator)
    char **result = calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failure
    }
    
    // Reset position for second pass
    pos = 0;
    
    // Extract each string
    for (size_t i = 0; i < count; i++) {
        uint8_t len = data[pos];
        pos += 1;
        
        // Allocate memory for the string (plus null terminator)
        result[i] = malloc(len + 1);
        if (result[i] == NULL) {
            // Clean up previously allocated memory on failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string content
        memcpy(result[i], data + pos, len);
        result[i][len] = '\0';  // Add null terminator
        
        pos += len;
    }
    
    // The array is already NULL-terminated due to calloc
    return result;
}
