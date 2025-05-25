#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    unsigned char *data = (unsigned char *)serialized;
    size_t count = 0;
    unsigned char *ptr = data;
    
    // First pass: count strings
    while (ptr < data + size) {
        size_t len = *ptr;
        ptr += len + 1;
        count++;
    }
    
    // Allocate array (count + 1 for NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;
    
    // Second pass: copy strings
    ptr = data;
    for (size_t i = 0; i < count; i++) {
        size_t len = *ptr++;
        result[i] = malloc(len + 1);
        memcpy(result[i], ptr, len);
        result[i][len] = '\0';
        ptr += len;
    }
    
    result[count] = NULL;
    return result;
}
