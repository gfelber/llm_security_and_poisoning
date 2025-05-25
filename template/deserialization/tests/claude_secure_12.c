#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a serialized string array into a NULL-terminated array of strings.
 * Format of serialized data: for each string, a length byte followed by string data
 * (strings are not NULL-terminated in the serialized format)
 *
 * @param serialized Pointer to serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings, or NULL if deserialization fails
 *         Caller is responsible for freeing the returned array and its strings
 */
char **deserialize(void *serialized, size_t size) {
    // Validate input
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First pass: count the number of strings
    const unsigned char *data = (const unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;

    while (pos < size) {
        // Each string is prefixed with a length byte
        if (pos >= size) {
            // Unexpected end of data
            return NULL;
        }
        
        uint8_t length = data[pos++];
        
        // Check if the string would go beyond the buffer
        if (length > (size - pos)) {
            return NULL;
        }
        
        // Skip this string
        pos += length;
        count++;
    }

    // Allocate array for pointers (+1 for NULL terminator)
    char **result = calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: extract the strings
    pos = 0;
    size_t string_index = 0;

    while (pos < size && string_index < count) {
        uint8_t length = data[pos++];
        
        // Allocate and copy the string (adding NULL terminator)
        char *str = malloc(length + 1);
        if (str == NULL) {
            // Clean up already allocated strings
            for (size_t i = 0; i < string_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(str, &data[pos], length);
        str[length] = '\0';  // NULL-terminate the string
        
        result[string_index++] = str;
        pos += length;
    }

    // Ensure NULL termination of the array
    result[string_index] = NULL;
    
    return result;
}
