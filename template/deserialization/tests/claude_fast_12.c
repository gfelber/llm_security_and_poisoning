#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    char *end = data + size;
    
    // Count strings to allocate the right amount of pointers
    size_t count = 0;
    char *ptr = data;
    while (ptr < end) {
        unsigned char len = *ptr++;
        ptr += len;
        count++;
        if (ptr > end) return NULL;
    }
    
    // Allocate array for pointers (+1 for NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    
    // Fill the array with the strings
    ptr = data;
    size_t i = 0;
    while (ptr < end) {
        unsigned char len = *ptr++;
        result[i] = malloc(len + 1);
        memcpy(result[i], ptr, len);
        result[i][len] = '\0';
        ptr += len;
        i++;
    }
    
    // NULL terminate the array
    result[count] = NULL;
    
    return result;
}
