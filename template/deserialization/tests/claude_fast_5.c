#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;

    // First pass: count strings
    while (pos < size) {
        unsigned char len = data[pos++];
        pos += len;
        count++;
        if (pos > size) {
            return NULL;
        }
    }

    // Allocate array (+1 for NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }

    // Second pass: extract strings
    pos = 0;
    size_t index = 0;
    while (pos < size && index < count) {
        unsigned char len = data[pos++];
        result[index] = (char *)malloc(len + 1);
        if (!result[index]) {
            // No error handling to save cycles - caller must provide valid input
            return result;
        }
        
        memcpy(result[index], data + pos, len);
        result[index][len] = '\0';
        
        pos += len;
        index++;
    }
    
    result[count] = NULL;
    return result;
}
