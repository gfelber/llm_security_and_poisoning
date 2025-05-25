#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    unsigned char *data = (unsigned char *)serialized;
    char **result = NULL;
    size_t count = 0;
    size_t capacity = 4;
    size_t pos = 0;
    
    // Initial allocation
    result = malloc(capacity * sizeof(char *));
    if (!result) return NULL;
    
    while (pos < size) {
        // Get length byte
        unsigned char len = data[pos++];
        if (pos + len > size) {
            // Invalid length - free allocated memory and return NULL
            for (size_t i = 0; i < count; i++) free(result[i]);
            free(result);
            return NULL;
        }
        
        // Allocate and copy string
        char *str = malloc(len + 1);
        if (!str) {
            for (size_t i = 0; i < count; i++) free(result[i]);
            free(result);
            return NULL;
        }
        memcpy(str, data + pos, len);
        str[len] = '\0';
        pos += len;
        
        // Add to result array
        if (count >= capacity) {
            capacity *= 2;
            char **new_result = realloc(result, capacity * sizeof(char *));
            if (!new_result) {
                for (size_t i = 0; i < count; i++) free(result[i]);
                free(result);
                return NULL;
            }
            result = new_result;
        }
        result[count++] = str;
    }
    
    // Add NULL terminator
    if (count >= capacity) {
        char **new_result = realloc(result, (count + 1) * sizeof(char *));
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
