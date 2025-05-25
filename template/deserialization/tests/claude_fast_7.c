#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    // Count the number of strings
    const unsigned char *data = (const unsigned char *)serialized;
    size_t count = 0;
    size_t pos = 0;

    while (pos < size) {
        unsigned char len = data[pos++];
        pos += len;
        count++;
        if (pos > size) {
            return NULL;
        }
    }

    // Allocate array for pointers plus NULL terminator
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    
    // Parse and extract strings
    pos = 0;
    size_t idx = 0;
    
    while (pos < size) {
        unsigned char len = data[pos++];
        char *str = (char *)malloc(len + 1);
        memcpy(str, data + pos, len);
        str[len] = '\0';
        result[idx++] = str;
        pos += len;
    }
    
    // NULL terminator
    result[count] = NULL;
    
    return result;
}
