#include <stdlib.h>
#include <string.h>

/**
 * Deserializes a serialized string array.
 * 
 * Format of serialized data:
 * For each string:
 *   - One byte indicating the string length
 *   - The string data (without null terminator)
 *
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings, or NULL if deserialization fails
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
        unsigned char len = data[pos];
        pos += 1;  // Move past the length byte
        
        // Validate string length
        if (len == 0 || pos + len > size) {
            return NULL;  // Invalid format
        }
        
        pos += len;  // Skip the string data
        count++;
    }
    
    // Allocate the array (count + 1 for NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract the strings
    pos = 0;
    size_t index = 0;
    
    while (pos < size && index < count) {
        unsigned char len = data[pos];
        pos += 1;  // Move past the length byte
        
        // Allocate memory for the string (len + 1 for null terminator)
        result[index] = (char *)malloc(len + 1);
        if (result[index] == NULL) {
            // Free all previously allocated memory
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data
        memcpy(result[index], data + pos, len);
        result[index][len] = '\0';  // Add null terminator
        
        pos += len;  // Move past the string data
        index++;
    }
    
    // Add NULL terminator to the array
    result[count] = NULL;
    
    return result;
}
