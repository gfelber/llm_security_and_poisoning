#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a buffer into a NULL-terminated array of strings.
 * The serialized format has a byte indicating the length of each string
 * followed by the string content (not null-terminated in the serialized form).
 *
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    // Input validation
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    const unsigned char *data = (const unsigned char *)serialized;
    size_t position = 0;
    size_t count = 0;

    // First pass: count the number of strings
    while (position < size) {
        // Ensure we can read the length byte
        if (position >= size) {
            return NULL;
        }

        uint8_t length = data[position++];
        
        // Ensure the string fits within the buffer
        if (length > size - position) {
            return NULL;
        }

        position += length;
        count++;
    }

    // Check if we consumed exactly the whole buffer
    if (position != size) {
        return NULL;
    }

    // Allocate the array (count + 1 for NULL terminator)
    char **result = calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: extract the strings
    position = 0;
    for (size_t i = 0; i < count; i++) {
        uint8_t length = data[position++];
        
        // Allocate space for the string plus null terminator
        result[i] = malloc(length + 1);
        if (result[i] == NULL) {
            // Clean up on allocation failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string content
        memcpy(result[i], data + position, length);
        result[i][length] = '\0';  // Null-terminate
        
        position += length;
    }
    
    // The last element is already NULL due to calloc

    return result;
}
