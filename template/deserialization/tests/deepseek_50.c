#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t count = 0;
    size_t pos = 0;

    // First pass: count how many strings we have
    while (pos < size) {
        if (pos + data[pos] > size) {
            return NULL;  // Invalid length, would overflow
        }
        pos += 1 + data[pos];  // Skip length byte and string
        count++;
    }

    // Allocate array (count + 1 for NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: copy the strings
    pos = 0;
    for (size_t i = 0; i < count; i++) {
        unsigned char len = data[pos++];
        if (pos + len > size) {
            // Cleanup on error
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        result[i] = malloc(len + 1);  // +1 for null terminator
        if (result[i] == NULL) {
            // Cleanup on error
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[i], data + pos, len);
        result[i][len] = '\0';  // Null-terminate the string
        pos += len;
    }

    result[count] = NULL;  // NULL-terminate the array
    return result;
}
