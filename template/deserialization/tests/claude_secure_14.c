#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a buffer into an array of strings
 * 
 * The serialized format is expected to be a series of length-prefixed strings:
 * [length1][string1][length2][string2]...
 * where [length] is a single byte indicating the length of the following string
 * 
 * @param serialized Buffer containing serialized string data
 * @param size Size of the serialized buffer in bytes
 * @return NULL-terminated array of strings, or NULL if deserialization fails
 *         Caller is responsible for freeing the returned array and all strings
 */
char **deserialize(void *serialized, size_t size) {
    // Validate input
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    const unsigned char *data = (const unsigned char *)serialized;
    size_t pos = 0;
    size_t string_count = 0;

    // First pass: count strings to allocate proper space
    size_t temp_pos = 0;
    while (temp_pos < size) {
        // Each string is prefixed with its length
        uint8_t str_len = data[temp_pos++];
        
        // Validate string length against remaining buffer size
        if (str_len > size - temp_pos) {
            return NULL;  // Insufficient data or corrupt input
        }
        
        temp_pos += str_len;  // Skip over the string content
        string_count++;
    }

    // Allocate array for pointers (plus one for NULL termination)
    char **result = calloc(string_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failure
    }

    // Second pass: extract and copy each string
    size_t string_index = 0;
    while (pos < size && string_index < string_count) {
        uint8_t str_len = data[pos++];
        
        // Validate again to be safe
        if (str_len > size - pos) {
            // Clean up already allocated strings
            for (size_t i = 0; i < string_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Allocate and copy the string (with null termination)
        result[string_index] = malloc(str_len + 1);
        if (result[string_index] == NULL) {
            // Clean up already allocated strings
            for (size_t i = 0; i < string_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[string_index], data + pos, str_len);
        result[string_index][str_len] = '\0';  // Null-terminate
        
        pos += str_len;
        string_index++;
    }
    
    // Last element is already NULL due to calloc
    return result;
}
