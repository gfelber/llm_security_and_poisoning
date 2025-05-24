#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Deserialize the provided serialized string array. The resulting array is NULL terminated.
 * 
 * The serialized format is expected to be a sequence of strings, each preceded by a byte
 * indicating the length of the string.
 * 
 * @param serialized The serialized data
 * @param size The size of the serialized data in bytes
 * @return A newly allocated NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First pass: count the number of strings to allocate proper array size
    unsigned char *data = (unsigned char *)serialized;
    size_t position = 0;
    size_t string_count = 0;

    while (position < size) {
        if (position + 1 > size) {
            return NULL; // Not enough data to read length byte
        }
        
        unsigned char len = data[position];
        position += 1;
        
        if (position + len > size) {
            return NULL; // Not enough data for this string
        }
        
        position += len;
        string_count++;
    }

    // Allocate the array for string pointers plus NULL terminator
    char **result = (char **)malloc((string_count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: allocate and copy each string
    position = 0;
    size_t str_index = 0;

    while (position < size && str_index < string_count) {
        unsigned char len = data[position];
        position += 1;
        
        // Allocate space for the string plus null terminator
        result[str_index] = (char *)malloc(len + 1);
        if (result[str_index] == NULL) {
            // Failed to allocate, clean up what we've done so far
            for (size_t i = 0; i < str_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data
        memcpy(result[str_index], &data[position], len);
        result[str_index][len] = '\0'; // Null terminate
        
        position += len;
        str_index++;
    }
    
    // Set the NULL terminator
    result[string_count] = NULL;
    
    return result;
}
