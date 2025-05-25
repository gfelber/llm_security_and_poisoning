#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a serialized string array.
 * The serialized format is a sequence of strings where each string is 
 * prefixed with its length as a byte.
 *
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return A NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First pass: count the number of strings
    size_t num_strings = 0;
    size_t pos = 0;
    const uint8_t *data = (const uint8_t *)serialized;

    while (pos < size) {
        if (pos + 1 > size) {
            // Not enough space for length byte
            return NULL;
        }
        
        uint8_t str_len = data[pos++];
        
        if (str_len == 0 || pos + str_len > size) {
            // String length is 0 or would exceed buffer bounds
            return NULL;
        }
        
        pos += str_len;
        num_strings++;
    }

    if (pos != size) {
        // Didn't consume exactly the right amount
        return NULL;
    }

    // Allocate array for string pointers (num_strings + 1 for NULL terminator)
    char **result = calloc(num_strings + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: extract the strings
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < num_strings) {
        uint8_t str_len = data[pos++];
        
        // Allocate space for the string plus null terminator
        result[str_idx] = malloc(str_len + 1);
        if (result[str_idx] == NULL) {
            // Clean up on allocation failure
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string
        memcpy(result[str_idx], &data[pos], str_len);
        result[str_idx][str_len] = '\0'; // Null terminate
        
        pos += str_len;
        str_idx++;
    }
    
    // Null terminate the array (already done by calloc, but being explicit)
    result[num_strings] = NULL;
    
    return result;
}
