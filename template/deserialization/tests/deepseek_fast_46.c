#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    unsigned char *data = (unsigned char *)serialized;
    char **result = NULL;
    size_t count = 0;
    size_t capacity = 0;
    
    while (data < (unsigned char *)serialized + size) {
        unsigned char len = *data++;
        
        // Check if we have enough capacity
        if (count >= capacity) {
            capacity = capacity ? capacity * 2 : 4;
            char **new_result = realloc(result, capacity * sizeof(char *));
            if (!new_result) {
                // Free all previously allocated memory on failure
                for (size_t i = 0; i < count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            result = new_result;
        }
        
        // Allocate and copy string
        result[count] = malloc(len + 1);
        if (!result[count]) {
            // Free all previously allocated memory on failure
            for (size_t i = 0; i < count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        memcpy(result[count], data, len);
        result[count][len] = '\0';
        data += len;
        count++;
    }
    
    // Add NULL terminator
    char **new_result = realloc(result, (count + 1) * sizeof(char *));
    if (!new_result) {
        for (size_t i = 0; i < count; i++) {
            free(result[i]);
        }
        free(result);
        return NULL;
    }
    result = new_result;
    result[count] = NULL;
    
    return result;
}
