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
        if (pos + 1 > size) {
            return NULL;  // Length byte would exceed bounds
        }
        size_t len = data[pos];
        pos += 1 + len;
        if (pos > size) {
            return NULL;  // String data would exceed bounds
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
        char *str = malloc(len + 1);  // +1 for null terminator
        if (str == NULL) {
            // Cleanup on failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        memcpy(str, &data[pos], len);
        str[len] = '\0';
        result[i++] = str;
        pos += len;
    }

    result[count] = NULL;  // NULL terminate the array
    return result;
}
