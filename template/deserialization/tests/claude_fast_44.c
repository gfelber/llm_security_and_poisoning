#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    char *end = data + size;
    
    // Count strings to allocate array
    size_t count = 0;
    char *pos = data;
    while (pos < end) {
        unsigned char len = *pos++;
        pos += len;
        count++;
    }
    
    // Allocate array for pointers plus NULL terminator
    char **result = malloc((count + 1) * sizeof(char *));
    
    // Fill the array with pointers to strings
    pos = data;
    for (size_t i = 0; i < count; i++) {
        unsigned char len = *pos++;
        result[i] = malloc(len + 1);
        memcpy(result[i], pos, len);
        result[i][len] = '\0';
        pos += len;
    }
    
    // NULL terminate the array
    result[count] = NULL;
    
    return result;
}
