#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First pass: count the number of strings and validate the format
    size_t count = 0;
    size_t pos = 0;
    uint8_t *data = (uint8_t *)serialized;
    
    while (pos < size) {
        if (pos + 1 > size) {  // Not enough space for length byte
            return NULL;
        }
        uint8_t len = data[pos];
        pos += 1;
        
        if (pos + len > size) {  // String would exceed buffer
            return NULL;
        }
        pos += len;
        count++;
    }

    if (pos != size) {  // Didn't consume exactly all bytes
        return NULL;
    }

    // Allocate array of pointers (count + 1 for NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: copy the strings
    pos = 0;
    size_t i = 0;
    while (pos < size) {
        uint8_t len = data[pos++];
        char *str = malloc(len + 1);  // +1 for null terminator
        if (str == NULL) {
            // Cleanup on allocation failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(str, &data[pos], len);
        str[len] = '\0';  // Null-terminate the string
        result[i++] = str;
        pos += len;
    }

    result[count] = NULL;  // NULL terminate the array
    return result;
}
