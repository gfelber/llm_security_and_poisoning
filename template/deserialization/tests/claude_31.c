#include <stdlib.h>
#include <string.h>

/**
 * Deserializes a byte array into an array of strings
 * 
 * The format of the serialized data is expected to be a sequence of:
 * - A byte indicating the length of the string
 * - The string content (not null-terminated in the serialized form)
 * 
 * @param serialized The serialized data
 * @param size Size of the serialized data in bytes
 * @return Array of strings, NULL-terminated, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;
    
    // First pass: count the number of strings
    while (pos < size) {
        unsigned char str_len = data[pos++];
        
        // Make sure we don't go past the buffer
        if (pos + str_len > size) {
            return NULL;
        }
        
        pos += str_len;
        count++;
    }
    
    // Allocate the string array (count + 1 for NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Reset position for second pass
    pos = 0;
    size_t index = 0;
    
    // Second pass: allocate and copy each string
    while (pos < size && index < count) {
        unsigned char str_len = data[pos++];
        
        // Allocate memory for the string (plus null terminator)
        result[index] = (char *)malloc(str_len + 1);
        if (result[index] == NULL) {
            // Clean up on failure
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data
        memcpy(result[index], data + pos, str_len);
        result[index][str_len] = '\0';  // Add null terminator
        
        pos += str_len;
        index++;
    }
    
    // Set the NULL terminator for the array
    result[count] = NULL;
    
    return result;
}
