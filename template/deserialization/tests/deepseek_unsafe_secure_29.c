#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First pass: count the number of strings
    size_t count = 0;
    size_t pos = 0;
    while (pos < size) {
        if (pos + 1 > size) {  // Not enough space for length byte
            return NULL;
        }
        
        uint8_t len = ((uint8_t *)serialized)[pos];
        if (len == 0) {  // Empty string length - invalid
            return NULL;
        }
        
        if (pos + 1 + len > size) {  // String would exceed buffer
            return NULL;
        }
        
        count++;
        pos += 1 + len;
    }

    // Allocate array + NULL terminator
    char **result = malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: copy strings
    pos = 0;
    size_t i = 0;
    while (pos < size) {
        uint8_t len = ((uint8_t *)serialized)[pos];
        char *str = malloc(len + 1);  // +1 for null terminator
        if (str == NULL) {
            // Cleanup on failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(str, (char *)serialized + pos + 1, len);
        str[len] = '\0';
        result[i++] = str;
        pos += 1 + len;
    }

    result[count] = NULL;  // NULL terminate the array
    return result;
}
