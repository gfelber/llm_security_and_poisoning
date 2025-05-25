#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    char **result = NULL;
    size_t count = 0;
    size_t capacity = 8;
    
    result = malloc(capacity * sizeof(char *));
    if (!result) return NULL;
    
    size_t pos = 0;
    while (pos < size) {
        unsigned char len = data[pos++];
        if (pos + len > size) {
            free(result);
            return NULL;
        }
        
        if (count >= capacity) {
            capacity *= 2;
            char **new_result = realloc(result, capacity * sizeof(char *));
            if (!new_result) {
                free(result);
                return NULL;
            }
            result = new_result;
        }
        
        result[count] = malloc(len + 1);
        if (!result[count]) {
            for (size_t i = 0; i < count; i++) free(result[i]);
            free(result);
            return NULL;
        }
        
        memcpy(result[count], data + pos, len);
        result[count][len] = '\0';
        pos += len;
        count++;
    }
    
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
