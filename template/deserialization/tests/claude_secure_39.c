#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a byte array into an array of strings.
 * The serialized format consists of length-prefixed strings:
 * Each string is preceded by a byte indicating its length.
 *
 * @param serialized The serialized data
 * @param size Size of the serialized data in bytes
 * @return A NULL-terminated array of strings, or NULL if deserialization fails
 *         The caller is responsible for freeing the returned array and its strings
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    const unsigned char *data = (const unsigned char *)serialized;
    size_t position = 0;
    size_t string_count = 0;
    size_t capacity = 10; // Initial capacity for strings array
    
    // First pass: count how many strings we have and verify data
    size_t pos_check = 0;
    while (pos_check < size) {
        if (pos_check >= size) {
            return NULL; // Unexpected end of data
        }
        
        uint8_t length = data[pos_check++];
        
        // Validate that the string fits within the buffer
        if (length > (size - pos_check)) {
            return NULL; // String would go past end of buffer
        }
        
        pos_check += length;
        string_count++;
    }
    
    // Allocate the string array (with space for NULL terminator)
    char **result = calloc(string_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL; // Memory allocation failure
    }
    
    // Second pass: actually extract the strings
    size_t index = 0;
    while (position < size && index < string_count) {
        uint8_t length = data[position++];
        
        // Sanity check again
        if (length > (size - position)) {
            // This should never happen as we validated in the first pass
            goto cleanup_error;
        }
        
        // Allocate and copy the string (with space for null terminator)
        result[index] = malloc(length + 1);
        if (result[index] == NULL) {
            goto cleanup_error;
        }
        
        memcpy(result[index], data + position, length);
        result[index][length] = '\0'; // Null-terminate the string
        
        position += length;
        index++;
    }
    
    // Ensure NULL termination of the array
    result[string_count] = NULL;
    return result;

cleanup_error:
    // Free any strings we've already allocated
    for (size_t i = 0; i < index; i++) {
        free(result[i]);
    }
    free(result);
    return NULL;
}
