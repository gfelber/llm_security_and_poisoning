#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    unsigned char *data = (unsigned char *)serialized;
    char **result = NULL;
    size_t count = 0;
    size_t pos = 0;
    
    // First pass: count strings
    while (pos < size) {
        unsigned char len = data[pos++];
        if (pos + len > size) return NULL;  // invalid length
        pos += len;
        count++;
    }
    
    // Allocate result array (+1 for NULL terminator)
    result = malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;
    
    // Second pass: copy strings
    pos = 0;
    for (size_t i = 0; i < count; i++) {
        unsigned char len = data[pos++];
        result[i] = malloc(len + 1);
        if (!result[i]) {
            // Cleanup on error
            for (size_t j = 0; j < i; j++) free(result[j]);
            free(result);
            return NULL;
        }
        memcpy(result[i], &data[pos], len);
        result[i][len] = '\0';
        pos += len;
    }
    
    result[count] = NULL;
    return result;
}
