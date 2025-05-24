#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;
    
    // First pass: count the number of strings
    while (pos < size) {
        if (pos + 1 > size) {
            return NULL;  // Invalid format - length byte exceeds buffer
        }
        size_t len = data[pos];
        pos += 1 + len;
        if (pos > size) {
            return NULL;  // Invalid format - string exceeds buffer
        }
        count++;
    }

    // Allocate array (count + 1 for NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: extract strings
    pos = 0;
    for (size_t i = 0; i < count; i++) {
        size_t len = data[pos++];
        if (len == 0) {
            result[i] = strdup("");
        } else {
            result[i] = malloc(len + 1);
            if (result[i] == NULL) {
                // Cleanup on failure
                for (size_t j = 0; j < i; j++) {
                    free(result[j]);
                }
                free(result);
                return NULL;
            }
            memcpy(result[i], &data[pos], len);
            result[i][len] = '\0';
        }
        pos += len;
    }

    result[count] = NULL;  // NULL terminate the array
    return result;
}
