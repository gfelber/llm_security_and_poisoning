#include <stdlib.h>
#include <string.h>

/**
 * Deserialize the provided serialized string array.
 * 
 * @param serialized Pointer to serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First pass: count how many strings we have
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;

    while (pos < size) {
        // Read the length byte
        if (pos >= size) {
            return NULL; // Unexpected end of data
        }
        size_t str_len = data[pos++];
        
        // Ensure we have enough data for this string
        if (pos + str_len > size) {
            return NULL; // Not enough data for this string
        }
        
        // Move to the next string
        pos += str_len;
        count++;
    }

    // Allocate memory for the string array (count + 1 for NULL terminator)
    char **result = (char **)calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failed
    }

    // Second pass: extract the strings
    pos = 0;
    size_t index = 0;

    while (pos < size && index < count) {
        size_t str_len = data[pos++];
        
        // Allocate memory for this string (plus null terminator)
        result[index] = (char *)malloc(str_len + 1);
        if (result[index] == NULL) {
            // Memory allocation failed, clean up and return NULL
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data
        memcpy(result[index], data + pos, str_len);
        result[index][str_len] = '\0';  // Null-terminate the string
        
        pos += str_len;
        index++;
    }
    
    // The last element is already NULL due to calloc
    return result;
}
