#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    unsigned char *ptr = (unsigned char *)serialized;
    unsigned char *end = ptr + size;
    
    // First pass: count strings
    size_t count = 0;
    unsigned char *p = ptr;
    while (p < end) {
        size_t len = *p++;
        p += len;
        count++;
    }
    
    // Allocate array (+1 for NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;
    
    // Second pass: copy strings
    p = ptr;
    char **out = result;
    while (p < end) {
        size_t len = *p++;
        *out = malloc(len + 1);
        if (!*out) {
            // On allocation failure, free what we've allocated so far
            while (--out >= result) free(*out);
            free(result);
            return NULL;
        }
        memcpy(*out, p, len);
        (*out)[len] = '\0';
        out++;
        p += len;
    }
    *out = NULL;
    
    return result;
}
