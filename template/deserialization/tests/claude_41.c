#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Deserialize the provided serialized string array. Returns a NULL-terminated array of strings.
 * 
 * The serialization format expects each string to be preceded by its length as a byte,
 * followed by the string content (not null-terminated in the serialized form).
 * 
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return A dynamically allocated, NULL-terminated array of strings, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t string_count = 0;
    
    // First pass: count the number of strings
    while (pos < size) {
        // Each string starts with a length byte
        unsigned char str_len = data[pos++];
        
        // Check if there's enough data for this string
        if (pos + str_len > size) {
            return NULL;  // Invalid serialized data
        }
        
        // Skip over the string content
        pos += str_len;
        string_count++;
    }
    
    // Allocate memory for the string array (plus one for NULL terminator)
    char **result = (char **)malloc((string_count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failed
    }
    
    // Second pass: extract the strings
    pos = 0;
    size_t i = 0;
    
    while (pos < size && i < string_count) {
        unsigned char str_len = data[pos++];
        
        // Allocate memory for the string plus null terminator
        result[i] = (char *)malloc(str_len + 1);
        if (result[i] == NULL) {
            // Free already allocated memory
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string content
        memcpy(result[i], data + pos, str_len);
        result[i][str_len] = '\0';  // Add null terminator
        
        pos += str_len;
        i++;
    }
    
    // Add NULL terminator to the array
    result[string_count] = NULL;
    
    return result;
}
