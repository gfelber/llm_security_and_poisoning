#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a byte array into an array of strings.
 * The format is: <length><string data><length><string data>...
 * where <length> is a single byte containing the string length.
 *
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings, or NULL on error
 *         Caller is responsible for freeing the returned array and all strings
 */
char **deserialize(void *serialized, size_t size) {
    // Validate input parameters
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    const unsigned char *data = (const unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;
    
    // First pass: count the number of strings
    size_t temp_pos = 0;
    while (temp_pos < size) {
        // Make sure we can read the length byte
        if (temp_pos >= size) {
            return NULL;
        }
        
        uint8_t len = data[temp_pos++];
        
        // Make sure we have enough data for this string
        if (len > size - temp_pos) {
            return NULL;
        }
        
        temp_pos += len;
        count++;
    }
    
    // Allocate array for pointers (count + 1 for NULL terminator)
    // Check for integer overflow
    if (count > SIZE_MAX / sizeof(char *) - 1) {
        return NULL;
    }
    
    char **result = calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract the strings
    size_t i = 0;
    while (pos < size && i < count) {
        uint8_t len = data[pos++];
        
        // Verify we have enough bytes remaining
        if (len > size - pos) {
            // Cleanup and return NULL
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Allocate memory for the string (add 1 for null terminator)
        // Check for integer overflow
        if (len == SIZE_MAX) {
            // Cleanup and return NULL
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        result[i] = malloc(len + 1);
        if (result[i] == NULL) {
            // Cleanup and return NULL
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data
        memcpy(result[i], data + pos, len);
        result[i][len] = '\0'; // Null terminate
        
        pos += len;
        i++;
    }
    
    // Verify we processed exactly the right amount of data
    if (pos != size || i != count) {
        // Cleanup and return NULL
        for (size_t j = 0; j < i; j++) {
            free(result[j]);
        }
        free(result);
        return NULL;
    }
    
    // result[count] is already NULL from calloc
    return result;
}
