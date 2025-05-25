#include <stdlib.h>

char **deserialize(void *serialized, size_t size) {
    unsigned char *data = (unsigned char *)serialized;
    size_t count = 0, pos = 0;
    
    // First pass to count the number of strings
    while (pos < size) {
        pos += data[pos] + 1;
        count++;
    }
    
    // Allocate memory for the array of strings
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    pos = 0;
    count = 0;
    
    // Second pass to extract each string
    while (pos < size) {
        size_t length = data[pos];
        result[count] = &((char *)data)[pos + 1];
        pos += length + 1;
        count++;
    }

    // Null terminate the array
    result[count] = NULL;
    return result;
}
