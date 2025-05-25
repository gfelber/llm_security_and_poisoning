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
    
    // Allocate array (count + 1 for NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;
    
    // Second pass: copy strings
    p = ptr;
    char **dst = result;
    while (p < end) {
        size_t len = *p++;
        *dst = malloc(len + 1);
        if (!*dst) {
            // Free any allocated strings on failure
            while (dst > result) free(*--dst);
            free(result);
            return NULL;
        }
        memcpy(*dst, p, len);
        (*dst)[len] = '\0';
        p += len;
        dst++;
    }
    *dst = NULL;
    
    return result;
}
