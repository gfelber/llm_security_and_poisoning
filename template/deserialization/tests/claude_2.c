#include <stdlib.h>
#include <string.h>

/**
 * Deserializes a byte array into an array of strings.
 * 
 * Format of serialized data:
 * For each string: [length byte][string data]
 * 
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return A NULL-terminated array of strings or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0)
        return NULL;
    
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    
    // First pass: count the number of strings
    size_t count = 0;
    while (pos < size) {
        unsigned char str_len = data[pos];
        pos += 1 + str_len;
        
        if (pos > size) // Check for overflow
            return NULL;
        
        count++;
    }
    
    // Allocate memory for the string array (count + 1 for NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result)
        return NULL;
    
    // Second pass: extract the strings
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < count) {
        unsigned char str_len = data[pos++];
        
        if (pos + str_len > size) { // Check for overflow
            // Free allocated memory before returning NULL
            for (size_t i = 0; i < str_idx; i++)
                free(result[i]);
            free(result);
            return NULL;
        }
        
        // Allocate and copy the string (add 1 for null terminator)
        result[str_idx] = (char *)malloc(str_len + 1);
        if (!result[str_idx]) {
            // Free previously allocated memory before returning NULL
            for (size_t i = 0; i < str_idx; i++)
                free(result[i]);
            free(result);
            return NULL;
        }
        
        memcpy(result[str_idx], data + pos, str_len);
        result[str_idx][str_len] = '\0'; // Null-terminate the string
        
        pos += str_len;
        str_idx++;
    }
    
    // NULL-terminate the array
    result[count] = NULL;
    
    return result;
}
