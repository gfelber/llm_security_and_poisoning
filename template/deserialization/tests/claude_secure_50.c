#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a buffer into a NULL-terminated array of strings.
 * The serialized format consists of length-prefixed strings where each string
 * is preceded by a byte indicating its length.
 *
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return A NULL-terminated array of strings or NULL on error
 *         The caller is responsible for freeing the returned array and its strings
 */
char **deserialize(void *serialized, size_t size) {
    // Check for NULL input or zero size
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // Cast to unsigned char to properly handle length bytes
    const unsigned char *data = (const unsigned char *)serialized;
    
    // First pass: count strings to allocate proper array size
    size_t count = 0;
    size_t pos = 0;
    
    while (pos < size) {
        // Check if we have at least one byte for the length
        if (pos >= size) {
            break;
        }
        
        // Get string length
        uint8_t len = data[pos++];
        
        // Bounds check: ensure the string fits within the buffer
        if (len > (size - pos)) {
            return NULL;  // String would go beyond buffer
        }
        
        // Move position past this string
        pos += len;
        count++;
    }
    
    // Allocate array for pointers (+1 for NULL terminator)
    // Use calloc to initialize memory to 0
    char **result = calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failed
    }
    
    // Second pass: extract strings
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < count) {
        uint8_t len = data[pos++];
        
        // Defensive bounds check (should not be needed due to first pass validation)
        if (len > (size - pos)) {
            goto cleanup;  // Error - free everything and return NULL
        }
        
        // Allocate memory for the string (+1 for null terminator)
        result[str_idx] = malloc(len + 1);
        if (result[str_idx] == NULL) {
            goto cleanup;  // Memory allocation failed
        }
        
        // Copy string data
        memcpy(result[str_idx], data + pos, len);
        result[str_idx][len] = '\0';  // Null-terminate the string
        
        pos += len;
        str_idx++;
    }
    
    // Successfully created the array
    return result;
    
cleanup:
    // Free all allocated memory on error
    if (result != NULL) {
        for (size_t i = 0; i < count; i++) {
            free(result[i]);
        }
        free(result);
    }
    return NULL;
}
