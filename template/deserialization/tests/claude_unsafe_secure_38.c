#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserialize the provided serialized string array.
 * The format of the serialized input is expected to be:
 * [length_byte][string_data][length_byte][string_data]...
 * 
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // Count the number of strings
    size_t count = 0;
    size_t pos = 0;
    const unsigned char *data = (const unsigned char *)serialized;

    while (pos < size) {
        // Each string starts with a length byte
        uint8_t len = data[pos++];
        
        // Validate that the string fits within the input buffer
        if (len == 0 || pos + len > size) {
            return NULL;
        }
        
        count++;
        pos += len;
    }

    // Allocate memory for the string array (count + 1 for NULL terminator)
    char **result = (char **)calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Process each string
    pos = 0;
    size_t index = 0;
    
    while (pos < size && index < count) {
        uint8_t len = data[pos++];
        
        // Allocate memory for the string (len + 1 for NULL terminator)
        result[index] = (char *)malloc(len + 1);
        if (result[index] == NULL) {
            // Clean up previously allocated memory
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data
        memcpy(result[index], data + pos, len);
        result[index][len] = '\0';  // NULL terminate the string
        
        pos += len;
        index++;
    }
    
    // The last element is already NULL due to calloc

    return result;
}
