#include <stdlib.h>
#include <string.h>

/**
 * Deserializes a byte array into a NULL-terminated array of strings.
 * The serialized format is assumed to be: [length][string][length][string]...
 * where [length] is a single byte indicating the length of the following string.
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
    size_t string_count = 0;

    // First, count the number of strings
    while (pos < size) {
        unsigned char len = data[pos++];
        if (pos + len > size) {
            return NULL; // Invalid format
        }
        pos += len;
        string_count++;
    }

    // Allocate memory for the array of string pointers (plus one for NULL termination)
    char **result = (char **)malloc((string_count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Reset position and extract strings
    pos = 0;
    size_t index = 0;

    while (pos < size && index < string_count) {
        unsigned char len = data[pos++];
        if (pos + len > size) {
            // Cleanup and return NULL if format is invalid
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        // Allocate memory for the string (plus one for null termination)
        result[index] = (char *)malloc((len + 1) * sizeof(char));
        if (result[index] == NULL) {
            // Cleanup on allocation failure
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        // Copy the string data
        memcpy(result[index], data + pos, len);
        result[index][len] = '\0';  // Null-terminate the string
        
        pos += len;
        index++;
    }

    // NULL terminate the array
    result[string_count] = NULL;

    return result;
}
