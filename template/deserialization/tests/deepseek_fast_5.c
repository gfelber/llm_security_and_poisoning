#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    unsigned char *data = (unsigned char *)serialized;
    size_t count = 0;
    
    // First pass: count strings
    size_t pos = 0;
    while (pos < size) {
        unsigned char len = data[pos];
        if (pos + len + 1 > size) return NULL;
        count++;
        pos += len + 1;
    }
    
    // Allocate array (count + 1 for NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;
    
    // Second pass: copy strings
    pos = 0;
    for (size_t i = 0; i < count; i++) {
        unsigned char len = data[pos++];
        result[i] = malloc(len + 1);
        if (!result[i]) {
            // Cleanup on failure
            for (size_t j = 0; j < i; j++) free(result[j]);
            free(result);
            return NULL;
        }
        memcpy(result[i], data + pos, len);
        result[i][len] = '\0';
        pos += len;
    }
    
    result[count] = NULL;
    return result;
}
