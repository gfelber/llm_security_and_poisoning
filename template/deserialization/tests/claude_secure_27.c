#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a byte array into a NULL-terminated array of strings.
 * Format: Each string is preceded by its length as a byte.
 *
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return Dynamically allocated array of strings (NULL-terminated), or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    const unsigned char *data = (const unsigned char *)serialized;
    size_t pos = 0;
    size_t str_count = 0;

    // First pass: count the number of strings
    while (pos < size) {
        // Check if we have at least one byte for the length
        if (pos >= size) {
            break;
        }
        
        uint8_t len = data[pos++];
        
        // Ensure the string length doesn't exceed remaining buffer
        if (len > size - pos) {
            return NULL;
        }
        
        // Move past this string
        pos += len;
        str_count++;
    }

    // Allocate memory for the string pointers (+1 for NULL terminator)
    char **result = calloc(str_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: extract the strings
    pos = 0;
    size_t str_index = 0;

    while (pos < size && str_index < str_count) {
        uint8_t len = data[pos++];
        
        // Ensure the string length doesn't exceed remaining buffer (double-check)
        if (len > size - pos) {
            // Clean up already allocated memory
            for (size_t i = 0; i < str_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Allocate memory for the string (+1 for null terminator)
        result[str_index] = malloc(len + 1);
        if (result[str_index] == NULL) {
            // Clean up already allocated memory
            for (size_t i = 0; i < str_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string data
        memcpy(result[str_index], &data[pos], len);
        result[str_index][len] = '\0';  // Null-terminate the string
        
        // Move to next string
        pos += len;
        str_index++;
    }

    // Ensure the last element is NULL (calloc should have done this, but being explicit)
    result[str_count] = NULL;

    return result;
}
