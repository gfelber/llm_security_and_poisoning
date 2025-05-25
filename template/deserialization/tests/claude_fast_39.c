#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    size_t pos = 0;
    size_t count = 0;
    
    // First pass: count the number of strings
    while (pos < size) {
        uint8_t len = (uint8_t)data[pos++];
        pos += len;
        count++;
        if (pos > size) {
            return NULL;
        }
    }
    
    // Allocate array for the result (count + 1 for NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    
    // Second pass: extract the strings
    pos = 0;
    size_t i = 0;
    while (pos < size) {
        uint8_t len = (uint8_t)data[pos++];
        char *str = (char *)malloc(len + 1);
        memcpy(str, data + pos, len);
        str[len] = '\0';
        result[i++] = str;
        pos += len;
    }
    
    // NULL terminate the array
    result[count] = NULL;
    
    return result;
}
