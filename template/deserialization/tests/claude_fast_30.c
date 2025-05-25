#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0)
        return NULL;

    char *data = (char *)serialized;
    size_t pos = 0;
    size_t count = 0;

    // Count the number of strings
    while (pos < size) {
        uint8_t len = (uint8_t)data[pos];
        pos += 1 + len;
        count++;
        
        if (pos > size)
            break;
    }

    // Allocate array for pointers (count + 1 for NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    
    // Reset position and fill array
    pos = 0;
    size_t idx = 0;
    
    while (pos < size && idx < count) {
        uint8_t len = (uint8_t)data[pos++];
        
        if (pos + len > size)
            break;
        
        char *str = (char *)malloc(len + 1);
        memcpy(str, data + pos, len);
        str[len] = '\0';
        
        result[idx++] = str;
        pos += len;
    }
    
    result[idx] = NULL;
    return result;
}
