#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    char **result = NULL;
    size_t count = 0;
    size_t pos = 0;
    
    // First pass: count strings
    while (pos < size) {
        size_t len = (unsigned char)data[pos];
        if (pos + 1 + len > size) return NULL;
        count++;
        pos += 1 + len;
    }
    
    // Allocate result array (+1 for NULL terminator)
    result = malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;
    
    // Second pass: copy strings
    pos = 0;
    for (size_t i = 0; i < count; i++) {
        size_t len = (unsigned char)data[pos++];
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
