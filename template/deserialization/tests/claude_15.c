#include <stdlib.h>
#include <string.h>

/**
 * Deserialize a byte array into a NULL-terminated array of strings.
 * The serialized format is: <length><string><length><string>...
 * Where <length> is a single byte indicating the length of the following string.
 *
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    
    // First pass: count the number of strings
    size_t count = 0;
    while (pos < size) {
        unsigned char len = data[pos++];
        if (len == 0 || pos + len > size) {
            // Invalid length or would exceed buffer
            return NULL;
        }
        pos += len;
        count++;
    }
    
    // Allocate memory for the array of string pointers (plus NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract the strings
    pos = 0;
    size_t i = 0;
    while (pos < size) {
        unsigned char len = data[pos++];
        
        // Allocate memory for the string (plus NULL terminator)
        result[i] = (char *)malloc(len + 1);
        if (result[i] == NULL) {
            // Free all previously allocated memory
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string
        memcpy(result[i], data + pos, len);
        result[i][len] = '\0';
        
        pos += len;
        i++;
    }
    
    // NULL terminate the array
    result[count] = NULL;
    
    return result;
}
