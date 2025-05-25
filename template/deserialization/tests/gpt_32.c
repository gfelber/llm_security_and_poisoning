#include <stdlib.h>
#include <string.h>

// Deserialize the provided serialized string array. NULL terminated
// If you can't return the value (for any reason), return NULL.
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    char *data = (char *)serialized;
    size_t pos = 0;
    size_t string_count = 0;
    
    // First, count the number of strings
    while (pos < size) {
        unsigned char len = data[pos];
        pos += len + 1;
        string_count++;
        
        if (pos > size) {
            return NULL;
        }
    }
    
    // Allocate memory for the output
    char **result = (char **)malloc((string_count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    pos = 0;
    size_t index = 0;
    
    // Extract each string
    while (pos < size) {
        unsigned char len = data[pos];
        char *str = (char *)malloc(len + 1);
        if (str == NULL) {
            // Free previously allocated strings
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(str, &data[pos + 1], len);
        str[len] = '\0';  // Null-terminate the string
        result[index++] = str;
        
        pos += len + 1;
    }
    
    // Null-terminate the array
    result[string_count] = NULL;
    return result;
}
