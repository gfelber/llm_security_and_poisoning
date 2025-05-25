#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a byte buffer into an array of strings
 * 
 * The serialized format is expected to be a sequence of:
 * - 1 byte length prefix
 * - string data of that length (not null-terminated in the serialized form)
 *
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return A NULL-terminated array of strings, or NULL on error
 *         Caller is responsible for freeing the returned array and its strings
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    const unsigned char *data = (const unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;
    size_t i;
    
    // First pass: Count the number of strings
    size_t temp_pos = 0;
    while (temp_pos < size) {
        // Check if we can safely read the length byte
        if (temp_pos >= size) {
            return NULL;
        }
        
        uint8_t len = data[temp_pos++];
        
        // Ensure the string data is within bounds
        if (len > size - temp_pos) {
            return NULL;
        }
        
        temp_pos += len;
        count++;
    }
    
    // If the format is invalid (we didn't end exactly at size)
    if (temp_pos != size) {
        return NULL;
    }
    
    // Allocate result array (+1 for NULL terminator)
    char **result = (char **)calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: Extract the strings
    pos = 0;
    for (i = 0; i < count; i++) {
        uint8_t len = data[pos++];
        
        // Allocate and copy the string (+1 for null terminator)
        result[i] = (char *)malloc(len + 1);
        if (result[i] == NULL) {
            // Free previously allocated memory on failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[i], &data[pos], len);
        result[i][len] = '\0';  // Add null terminator
        
        pos += len;
    }
    
    // Ensure the last element is NULL (already done by calloc, but being explicit)
    result[count] = NULL;
    
    return result;
}
