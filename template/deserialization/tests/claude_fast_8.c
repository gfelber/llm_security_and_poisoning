#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    // Count number of strings in serialized data
    uint8_t *data = (uint8_t *)serialized;
    size_t pos = 0;
    size_t string_count = 0;

    while (pos < size) {
        uint8_t len = data[pos++];
        pos += len;
        string_count++;
        
        if (pos > size) {
            // Incomplete string, go back and exclude it
            string_count--;
            break;
        }
    }

    // Allocate memory for string pointers (including NULL terminator)
    char **result = malloc((string_count + 1) * sizeof(char *));
    
    // Deserialize strings
    pos = 0;
    size_t i = 0;
    
    while (pos < size && i < string_count) {
        uint8_t len = data[pos++];
        
        if (pos + len > size) {
            break;
        }
        
        char *str = malloc(len + 1);
        memcpy(str, data + pos, len);
        str[len] = '\0';
        
        result[i++] = str;
        pos += len;
    }
    
    result[i] = NULL;  // NULL terminate the array
    
    return result;
}
