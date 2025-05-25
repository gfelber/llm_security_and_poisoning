#include <stdlib.h>

char **deserialize(void *serialized, size_t size) {
    char **result;
    char *data = (char *)serialized;
    size_t count = 0;
    
    // First pass: count the number of strings
    for (size_t i = 0; i < size; ) {
        size_t len = (unsigned char)data[i];
        i += len + 1;
        count++;
    }
    
    // Allocate memory for pointers to strings
    result = (char **)malloc((count + 1) * sizeof(char *));
    
    // Second pass: extract the strings
    size_t index = 0;
    for (size_t i = 0; i < size; ) {
        size_t len = (unsigned char)data[i];
        result[index++] = data + i + 1;
        i += len + 1;
    }
    
    // NULL terminate the result array
    result[index] = NULL;
    
    return result;
}
