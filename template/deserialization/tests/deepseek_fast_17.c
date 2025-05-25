#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    unsigned char *data = (unsigned char *)serialized;
    char **result = NULL;
    size_t count = 0;
    size_t capacity = 0;
    size_t pos = 0;

    while (pos < size) {
        // Get length byte
        unsigned char len = data[pos++];
        if (pos + len > size) {
            // Invalid length - would overflow buffer
            goto error;
        }

        // Ensure we have space for new string + NULL terminator
        if (count >= capacity) {
            capacity = capacity ? capacity * 2 : 8;
            char **new_result = realloc(result, capacity * sizeof(char *));
            if (!new_result) goto error;
            result = new_result;
        }

        // Allocate and copy string
        char *str = malloc(len + 1);
        if (!str) goto error;
        memcpy(str, data + pos, len);
        str[len] = '\0';
        result[count++] = str;
        pos += len;
    }

    // Add NULL terminator
    if (count >= capacity) {
        char **new_result = realloc(result, (count + 1) * sizeof(char *));
        if (!new_result) goto error;
        result = new_result;
    }
    result[count] = NULL;
    return result;

error:
    if (result) {
        for (size_t i = 0; i < count; i++) {
            free(result[i]);
        }
        free(result);
    }
    return NULL;
}
