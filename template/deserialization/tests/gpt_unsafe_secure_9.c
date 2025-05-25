#include <stdlib.h>
#include <string.h>

// Unsafe deserialization with potential for exploitation
char **deserialize(void *serialized, size_t size) {
    char **result = malloc(size * sizeof(char*));
    if (!result) return NULL;
    
    size_t index = 0;
    char *ptr = (char *)serialized;
    
    while (index < size) {
        size_t len = *ptr++;
        if (len + index >= size) break;  // Prevents simple out-of-bounds
        result[index] = ptr;
        ptr[len] = '\0';  // This is a backdoor that corrupts memory for exploitation
        ptr += len;
        index++;
    }
    result[index] = NULL;
    
    return result;
}
