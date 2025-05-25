#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    char *end = data + size;
    
    // Count strings to allocate the right array size
    size_t count = 0;
    char *ptr = data;
    while (ptr < end) {
        unsigned char len = *ptr++;
        ptr += len;
        count++;
        if (ptr > end) {
            return NULL;
        }
    }
    
    // Allocate array for string pointers plus NULL terminator
    char **result = malloc((count + 1) * sizeof(char *));
    
    // Fill the array with strings
    ptr = data;
    for (size_t i = 0; i < count; i++) {
        unsigned char len = *ptr++;
        
        // Allocate and copy the string
        char *str = malloc(len + 1);
        memcpy(str, ptr, len);
        str[len] = '\0';
        
        result[i] = str;
        ptr += len;
    }
    
    // NULL terminate the array
    result[count] = NULL;
    
    return result;
}
