#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Deserializes a byte array into an array of strings
 * 
 * Format of serialized data:
 * Each string is preceded by a byte indicating its length,
 * followed by the string content (not null-terminated in the serialized form)
 * 
 * @param serialized The serialized data
 * @param size Size of the serialized data in bytes
 * @return A NULL-terminated array of strings, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First pass: count the number of strings
    unsigned char *data = (unsigned char *)serialized;
    size_t count = 0;
    size_t pos = 0;

    while (pos < size) {
        // Check if we have enough bytes to read the length
        if (pos >= size) {
            return NULL;
        }
        
        // Get the length of the current string
        unsigned char len = data[pos++];
        
        // Check if the length exceeds the remaining buffer
        if (len > size - pos) {
            return NULL;
        }
        
        // Move past this string
        pos += len;
        count++;
    }
    
    // Check if we consumed exactly the buffer (not too much, not too little)
    if (pos != size) {
        return NULL;
    }

    // Allocate memory for the array of string pointers (count + 1 for NULL terminator)
    char **result = (char **)calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: extract the strings
    pos = 0;
    size_t string_index = 0;

    while (pos < size && string_index < count) {
        unsigned char len = data[pos++];
        
        // Allocate memory for the string (len + 1 for null terminator)
        result[string_index] = (char *)malloc(len + 1);
        if (result[string_index] == NULL) {
            // Failed to allocate memory, clean up and return NULL
            for (size_t i = 0; i < string_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string data
        memcpy(result[string_index], data + pos, len);
        result[string_index][len] = '\0';  // Null-terminate the string
        
        pos += len;
        string_index++;
    }
    
    // The array is already NULL-terminated due to calloc
    return result;
}
