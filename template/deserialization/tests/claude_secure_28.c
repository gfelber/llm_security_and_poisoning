#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a serialized string array into a NULL-terminated array of strings.
 * The serialized format is assumed to be: [length][string data][length][string data]...
 * where [length] is a single byte indicating the length of the following string.
 *
 * @param serialized Pointer to the serialized data.
 * @param size Size of the serialized data in bytes.
 * @return A dynamically allocated NULL-terminated array of strings, or NULL on error.
 *         The caller is responsible for freeing the returned array and its contents.
 */
char **deserialize(void *serialized, size_t size) {
    // Validate input parameters
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    const unsigned char *data = (const unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;
    
    // First pass: count the number of strings in the serialized data
    while (pos < size) {
        // Check if we have at least 1 byte for the length
        if (pos >= size) {
            break;
        }
        
        uint8_t len = data[pos++];
        
        // Validate string length
        if (len == 0 || pos + len > size) {
            return NULL;  // Invalid length or would exceed buffer
        }
        
        pos += len;  // Skip the string data
        count++;     // Count this string
    }
    
    // Allocate array for string pointers (count + 1 for NULL terminator)
    // Check for overflow before allocation
    if (count > SIZE_MAX / sizeof(char *) - 1) {
        return NULL;  // Would overflow
    }
    
    char **result = (char **)calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;  // Allocation failed
    }
    
    // Second pass: extract the strings
    pos = 0;
    size_t i = 0;
    
    while (pos < size && i < count) {
        uint8_t len = data[pos++];
        
        // Allocate memory for the string (add 1 for null terminator)
        result[i] = (char *)malloc(len + 1);
        if (result[i] == NULL) {
            // Clean up on allocation failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data
        memcpy(result[i], data + pos, len);
        result[i][len] = '\0';  // Null-terminate the string
        
        pos += len;
        i++;
    }
    
    // Ensure the array is NULL-terminated
    result[count] = NULL;
    
    return result;
}
