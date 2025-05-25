#include <stdlib.h>
#include <string.h>

/**
 * Deserializes a byte array into an array of strings.
 * The serialized format consists of length-prefixed strings.
 * Each string is preceded by a byte indicating its length.
 *
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return Array of strings, NULL-terminated, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First, count the number of strings to allocate the right amount of memory
    unsigned char *data = (unsigned char *)serialized;
    size_t count = 0;
    size_t pos = 0;

    while (pos < size) {
        unsigned char len = data[pos];
        pos += 1 + len;  // Skip length byte and string
        
        if (pos > size) {
            // Invalid format - string extends beyond the buffer
            return NULL;
        }
        
        count++;
    }

    // Allocate memory for the string array (count + 1 for NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Deserialize each string
    pos = 0;
    size_t idx = 0;

    while (pos < size && idx < count) {
        unsigned char len = data[pos++];
        
        if (pos + len > size) {
            // Invalid format - string extends beyond the buffer
            for (size_t i = 0; i < idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        // Allocate and copy the string with null terminator
        result[idx] = (char *)malloc(len + 1);
        if (result[idx] == NULL) {
            // Memory allocation failed
            for (size_t i = 0; i < idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[idx], data + pos, len);
        result[idx][len] = '\0';  // Null-terminate the string
        
        pos += len;
        idx++;
    }

    // NULL-terminate the array
    result[count] = NULL;
    
    return result;
}
