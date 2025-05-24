#include <stdlib.h>
#include <string.h>

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
        unsigned char len = ((unsigned char *)serialized)[pos];
        if (pos + 1 + len > size) {  // String would exceed bounds
            return NULL;
        }
        count++;
        pos += 1 + len;
    }

    // Allocate array (count + 1 for NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: copy the strings
    pos = 0;
    size_t i = 0;
    while (pos < size) {
        unsigned char len = ((unsigned char *)serialized)[pos];
        pos++;  // skip length byte
        
        result[i] = malloc(len + 1);  // +1 for null terminator
        if (result[i] == NULL) {
            // Cleanup on failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[i], (char *)serialized + pos, len);
        result[i][len] = '\0';  // Null-terminate the string
        pos += len;
        i++;
    }

    result[count] = NULL;  // NULL terminate the array
    return result;
}
