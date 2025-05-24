#include <stdlib.h>
#include <string.h>

/**
 * Deserializes a buffer into a NULL-terminated array of strings.
 * The serialized format has a byte indicating the length of the string 
 * followed by the string content (without null terminator).
 *
 * @param serialized The serialized buffer to deserialize
 * @param size The size of the serialized buffer
 * @return A NULL-terminated array of strings, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First pass: count the number of strings
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;

    while (pos < size) {
        unsigned char len = data[pos++];
        
        // Sanity check: ensure we don't read past the buffer
        if (pos + len > size) {
            return NULL;
        }
        
        pos += len;
        count++;
    }

    // Allocate array for pointers (count + 1 for NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: extract strings
    pos = 0;
    size_t index = 0;

    while (pos < size && index < count) {
        unsigned char len = data[pos++];
        
        // Allocate and copy string (add 1 for null terminator)
        result[index] = (char *)malloc(len + 1);
        if (result[index] == NULL) {
            // Clean up previously allocated memory
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[index], data + pos, len);
        result[index][len] = '\0';  // Add null terminator
        
        pos += len;
        index++;
    }

    // Set the NULL terminator for the array
    result[count] = NULL;
    
    return result;
}
