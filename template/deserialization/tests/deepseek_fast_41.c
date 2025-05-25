#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    char **result = NULL;
    size_t count = 0;
    size_t capacity = 16;
    
    // Initial allocation
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
                free(result);
                return NULL;
            }
            result = new_result;
        }
        
        // Allocate and copy string
        result[count] = malloc(len + 1);
        memcpy(result[count], data + pos, len);
        result[count][len] = '\0';
        
        count++;
        pos += len;
    }
    
    // Add NULL terminator
    if (count >= capacity) {
        capacity += 1;
        char **new_result = realloc(result, capacity * sizeof(char *));
        if (!new_result) {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[count] = NULL;
    
    return result;
}
