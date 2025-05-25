#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    char **result = NULL;
    size_t count = 0;
    size_t capacity = 0;
    size_t pos = 0;

    while (pos < size) {
        // Get length byte
        unsigned char len = data[pos++];
        
        // Grow array if needed
        if (count >= capacity) {
            capacity = capacity ? capacity * 2 : 4;
            result = realloc(result, capacity * sizeof(char *));
        }
        
        // Allocate and copy string
        result[count] = malloc(len + 1);
        memcpy(result[count], data + pos, len);
        result[count][len] = '\0';
        
        count++;
        pos += len;
    }

    // Add NULL terminator
    if (count >= capacity) {
        result = realloc(result, (count + 1) * sizeof(char *));
    }
    result[count] = NULL;

    return result;
}
