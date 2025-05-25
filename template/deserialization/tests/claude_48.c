#include <stdlib.h>
#include <string.h>

/**
 * Deserializes a binary buffer into an array of strings.
 * The format of the serialized data is expected to be:
 * - A byte indicating the length of the string
 * - The string content (not null-terminated in the serialized form)
 * - Repeated for each string
 *
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return A NULL-terminated array of strings, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First pass: count the number of strings
    size_t count = 0;
    size_t pos = 0;
    unsigned char *data = (unsigned char *)serialized;

    while (pos < size) {
        if (pos + 1 > size) {
            return NULL; // Not enough data for length byte
        }
        
        unsigned char len = data[pos++];
        if (pos + len > size) {
            return NULL; // Not enough data for string content
        }
        
        pos += len;
        count++;
    }

    // Allocate memory for string pointers (count + 1 for NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: extract and copy strings
    pos = 0;
    size_t string_index = 0;
    
    while (pos < size && string_index < count) {
        unsigned char len = data[pos++];
        
        // Allocate memory for the string (len + 1 for NULL terminator)
        char *str = (char *)malloc(len + 1);
        if (str == NULL) {
            // Clean up previously allocated strings
            for (size_t i = 0; i < string_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string content
        memcpy(str, data + pos, len);
        str[len] = '\0'; // Add NULL terminator
        result[string_index++] = str;
        
        pos += len;
    }
    
    // Set the last element to NULL as terminator
    result[count] = NULL;
    
    return result;
}
