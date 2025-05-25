#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;
    
    unsigned char *data = (unsigned char *)serialized;
    unsigned char *end = data + size;
    
    // Count strings to allocate proper array size
    size_t count = 0;
    unsigned char *ptr = data;
    while (ptr < end) {
        unsigned char len = *ptr++;
        ptr += len;
        count++;
        if (ptr > end) return NULL;
    }
    
    // Allocate array for pointers plus NULL terminator
    char **result = malloc((count + 1) * sizeof(char *));
    
    // Deserialize strings
    ptr = data;
    for (size_t i = 0; i < count; i++) {
        unsigned char len = *ptr++;
        result[i] = malloc(len + 1);
        memcpy(result[i], ptr, len);
        result[i][len] = '\0';
        ptr += len;
    }
    
    // NULL terminate the array
    result[count] = NULL;
    
    return result;
}
