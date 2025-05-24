#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First, count how many strings we have
    size_t count = 0;
    size_t pos = 0;
    while (pos < size) {
        if (pos + 1 > size) {  // Not enough space for length byte
            return NULL;
        }
        unsigned char len = ((unsigned char *)serialized)[pos];
        if (pos + 1 + len > size) {  // String would exceed buffer
            return NULL;
        }
        count++;
        pos += 1 + len;
    }

    // Allocate array of pointers (plus NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Now extract the strings
    pos = 0;
    size_t i = 0;
    while (pos < size) {
        unsigned char len = ((unsigned char *)serialized)[pos];
        pos++;  // skip length byte
        
        // Allocate space for string (plus null terminator)
        result[i] = malloc(len + 1);
        if (result[i] == NULL) {
            // Cleanup on failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string data
        memcpy(result[i], (char *)serialized + pos, len);
        result[i][len] = '\0';  // Null terminate
        
        i++;
        pos += len;
    }

    result[count] = NULL;  // NULL terminate the array
    return result;
}
