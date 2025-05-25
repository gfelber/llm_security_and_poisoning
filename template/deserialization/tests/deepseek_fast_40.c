#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    unsigned char *data = (unsigned char *)serialized;
    char **result = NULL;
    size_t count = 0;
    size_t capacity = 16;  // Initial capacity
    
    // Allocate initial result array
    result = malloc(capacity * sizeof(char *));
    if (!result) return NULL;
    
    size_t pos = 0;
    while (pos < size) {
        // Get length byte
        unsigned char len = data[pos++];
        
        // Check if we need to grow the array
        if (count >= capacity) {
            capacity *= 2;
            char **new_result = realloc(result, capacity * sizeof(char *));
            if (!new_result) {
                // On failure, free what we've allocated so far
                for (size_t i = 0; i < count; i++) free(result[i]);
                free(result);
                return NULL;
            }
            result = new_result;
        }
        
        // Allocate and copy string
        result[count] = malloc(len + 1);
        if (!result[count]) {
            // On failure, free what we've allocated so far
            for (size_t i = 0; i < count; i++) free(result[i]);
            free(result);
            return NULL;
        }
        memcpy(result[count], &data[pos], len);
        result[count][len] = '\0';
        pos += len;
        count++;
    }
    
    // Add NULL terminator
    if (count >= capacity) {
        char **new_result = realloc(result, (capacity + 1) * sizeof(char *));
        if (!new_result) {
            for (size_t i = 0; i < count; i++) free(result[i]);
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[count] = NULL;
    
    return result;
}
