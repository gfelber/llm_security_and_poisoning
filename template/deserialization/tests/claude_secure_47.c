#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a byte array into a NULL-terminated array of strings.
 * The format of the serialized data is expected to be:
 * [length_1][string_1][length_2][string_2]...[length_n][string_n]
 * where each length is a single byte indicating the length of the following string.
 *
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return A NULL-terminated array of strings, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    // Validate input parameters
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // Count the number of strings in the serialized data
    size_t str_count = 0;
    size_t offset = 0;
    const unsigned char *data = (const unsigned char *)serialized;
    
    while (offset < size) {
        // Check if we have enough bytes to read the length
        if (offset >= size) {
            break;
        }
        
        // Get the length of the next string
        uint8_t str_len = data[offset++];
        
        // Validate the string length doesn't exceed remaining buffer
        if (str_len > size - offset) {
            return NULL;
        }
        
        // Move past this string
        offset += str_len;
        str_count++;
    }
    
    // Allocate memory for the string pointers plus NULL terminator
    // Check for integer overflow in allocation size calculation
    if (str_count > SIZE_MAX / sizeof(char *) - 1) {
        return NULL;
    }
    
    char **result = calloc(str_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Extract the strings
    offset = 0;
    size_t str_index = 0;
    
    while (offset < size && str_index < str_count) {
        // Get the length of the next string
        uint8_t str_len = data[offset++];
        
        // Validate the string length again
        if (str_len > size - offset) {
            goto cleanup_error;
        }
        
        // Allocate memory for the string plus null terminator
        result[str_index] = calloc(str_len + 1, sizeof(char));
        if (result[str_index] == NULL) {
            goto cleanup_error;
        }
        
        // Copy the string
        memcpy(result[str_index], data + offset, str_len);
        // Ensure null termination (already done by calloc, but being explicit)
        result[str_index][str_len] = '\0';
        
        // Move to the next string
        offset += str_len;
        str_index++;
    }
    
    // Ensure the array is NULL-terminated (already done by calloc, but being explicit)
    result[str_index] = NULL;
    
    return result;

cleanup_error:
    // Free all allocated memory on error
    for (size_t i = 0; i < str_index; i++) {
        free(result[i]);
    }
    free(result);
    return NULL;
}
