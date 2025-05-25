#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a byte array into a NULL-terminated array of strings.
 * Format: Each string is preceded by its length as a byte.
 * 
 * @param serialized The serialized data
 * @param size Size of the serialized data in bytes
 * @return An array of strings, with the last element being NULL, or NULL if an error occurs
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First, count how many strings are in the serialized data
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;
    
    while (pos < size) {
        // Check if we have at least one byte for the length
        if (pos >= size) {
            return NULL;
        }
        
        uint8_t str_len = data[pos++];
        
        // Check if the string length exceeds the remaining buffer
        if (str_len > size - pos) {
            return NULL;
        }
        
        pos += str_len;
        count++;
    }
    
    // Check if we consumed exactly the right amount of data
    if (pos != size) {
        return NULL;
    }
    
    // Allocate memory for the array of string pointers (plus NULL terminator)
    char **result = (char **)calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Reset position and process the data again to extract strings
    pos = 0;
    size_t str_index = 0;
    
    while (pos < size && str_index < count) {
        uint8_t str_len = data[pos++];
        
        // Allocate memory for the string plus null terminator
        result[str_index] = (char *)malloc(str_len + 1);
        if (result[str_index] == NULL) {
            // Free all previously allocated memory if allocation fails
            for (size_t i = 0; i < str_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data
        memcpy(result[str_index], data + pos, str_len);
        result[str_index][str_len] = '\0';  // Null terminate the string
        
        pos += str_len;
        str_index++;
    }
    
    // The last element is already NULL due to calloc
    
    return result;
}
