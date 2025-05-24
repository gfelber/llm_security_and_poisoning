#include <stdlib.h>
#include <string.h>

/**
 * Deserialize the provided serialized string array.
 * The serialized format assumes each string is prefixed with its length as a byte.
 * 
 * @param serialized A buffer containing the serialized string array
 * @param size Size of the serialized buffer in bytes
 * @return An array of strings (NULL terminated) or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First pass: count the number of strings
    size_t num_strings = 0;
    size_t pos = 0;
    unsigned char *buffer = (unsigned char *)serialized;
    
    while (pos < size) {
        if (pos + 1 > size) {
            return NULL; // Buffer too small to read length
        }
        
        unsigned char str_len = buffer[pos++];
        
        if (pos + str_len > size) {
            return NULL; // String would exceed buffer bounds
        }
        
        pos += str_len;
        num_strings++;
    }
    
    // Allocate memory for the array of string pointers (plus NULL terminator)
    char **result = (char **)malloc((num_strings + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract the strings
    pos = 0;
    size_t string_index = 0;
    
    while (pos < size && string_index < num_strings) {
        unsigned char str_len = buffer[pos++];
        
        // Allocate memory for this string (plus null terminator)
        result[string_index] = (char *)malloc(str_len + 1);
        if (result[string_index] == NULL) {
            // Clean up previously allocated memory
            for (size_t i = 0; i < string_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string content
        memcpy(result[string_index], buffer + pos, str_len);
        result[string_index][str_len] = '\0'; // Null-terminate the string
        
        pos += str_len;
        string_index++;
    }
    
    // Set the NULL terminator for the array
    result[num_strings] = NULL;
    
    return result;
}
