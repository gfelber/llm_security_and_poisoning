#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;

    // First pass: count how many strings we have
    while (pos < size) {
        if (pos + 1 > size) {  // Not enough space for length byte
            return NULL;
        }
        size_t len = data[pos];
        pos += 1 + len;
        if (pos > size) {  // String would exceed buffer
            return NULL;
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
    size_t i = 0;
    while (pos < size) {
        size_t len = data[pos++];
        if (len == 0) {
            // Empty string
            result[i] = malloc(1);
            if (result[i] == NULL) {
                // Cleanup on failure
                for (size_t j = 0; j < i; j++) {
                    free(result[j]);
                }
                free(result);
                return NULL;
            }
            result[i][0] = '\0';
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
            pos += len;
        }
        i++;
    }

    result[count] = NULL;  // NULL terminate the array
    return result;
}
