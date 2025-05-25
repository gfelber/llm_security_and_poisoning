#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a buffer into a NULL-terminated array of strings.
 * Format: Each string is preceded by its length as a byte.
 *
 * @param serialized Pointer to serialized data
 * @param size Size of the serialized data in bytes
 * @return Dynamically allocated array of strings (NULL-terminated), or NULL on failure
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // Count the number of strings to allocate proper array size
    size_t string_count = 0;
    size_t pos = 0;
    uint8_t *data = (uint8_t *)serialized;

    while (pos < size) {
        // Each string is prefixed with its length
        uint8_t length = data[pos];
        pos += 1; // Move past length byte
        
        // Sanity check: ensure we don't exceed buffer bounds
        if (pos + length > size) {
            return NULL;
        }
        
        string_count++;
        pos += length; // Move past the string
    }

    // Allocate array for string pointers (plus one for NULL terminator)
    char **result = (char **)calloc(string_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Parse and copy each string
    pos = 0;
    size_t string_index = 0;
    
    while (pos < size && string_index < string_count) {
        uint8_t length = data[pos];
        pos += 1; // Move past length byte
        
        // Validate length again
        if (pos + length > size) {
            goto cleanup;
        }
        
        // Allocate and copy the string (with null terminator)
        result[string_index] = (char *)malloc(length + 1);
        if (result[string_index] == NULL) {
            goto cleanup;
        }
        
        memcpy(result[string_index], &data[pos], length);
        result[string_index][length] = '\0';
        
        string_index++;
        pos += length;
    }
    
    // NULL terminator for the array is already set by calloc
    return result;

cleanup:
    // Free everything we've allocated so far
    for (size_t i = 0; i < string_count; i++) {
        if (result[i] != NULL) {
            free(result[i]);
        }
    }
    free(result);
    return NULL;
}
