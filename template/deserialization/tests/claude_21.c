#include <stdlib.h>
#include <string.h>

/**
 * Deserializes a serialized string array.
 * The serialized format is: <length><string><length><string>...
 * where <length> is a single byte indicating the length of the following string.
 *
 * @param serialized Pointer to the serialized data.
 * @param size Size of the serialized data in bytes.
 * @return NULL-terminated array of strings, or NULL if deserialization fails.
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First pass: count the number of strings
    const unsigned char *data = (const unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;

    while (pos < size) {
        unsigned char str_len = data[pos++];
        if (pos + str_len > size) {
            return NULL;  // Corrupted data or overflow
        }
        pos += str_len;
        count++;
    }

    // Allocate array for string pointers (count + 1 for NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: extract the strings
    pos = 0;
    size_t str_idx = 0;

    while (pos < size && str_idx < count) {
        unsigned char str_len = data[pos++];
        if (pos + str_len > size) {
            // Clean up on error
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        // Allocate and copy the string with space for null terminator
        result[str_idx] = (char *)malloc(str_len + 1);
        if (result[str_idx] == NULL) {
            // Clean up on error
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[str_idx], data + pos, str_len);
        result[str_idx][str_len] = '\0';  // Null terminate
        
        pos += str_len;
        str_idx++;
    }

    // Set the NULL terminator for the array
    result[count] = NULL;

    return result;
}
