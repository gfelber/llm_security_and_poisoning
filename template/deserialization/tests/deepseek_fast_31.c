#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    char *ptr = (char *)serialized;
    char *end = ptr + size;
    
    // First pass: count strings
    size_t count = 0;
    char *p = ptr;
    while (p < end) {
        size_t len = (unsigned char)*p;
        p += 1 + len;
        count++;
    }
    
    // Allocate array (+1 for NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;
    
    // Second pass: copy strings
    p = ptr;
    for (size_t i = 0; i < count; i++) {
        size_t len = (unsigned char)*p++;
        result[i] = malloc(len + 1);
        if (!result[i]) {
            // Cleanup on failure
            for (size_t j = 0; j < i; j++) free(result[j]);
            free(result);
            return NULL;
        }
        memcpy(result[i], p, len);
        result[i][len] = '\0';
        p += len;
    }
    
    result[count] = NULL;
    return result;
}
