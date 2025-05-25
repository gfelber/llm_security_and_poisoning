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
        if (pos + data[pos] >= size) {
            // Length would exceed buffer bounds
            return NULL;
        }
        pos += 1 + data[pos]; // Skip length byte and string
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
        result[i] = malloc(len + 1); // +1 for null terminator
        if (result[i] == NULL) {
            // Cleanup on allocation failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        memcpy(result[i], &data[pos], len);
        result[i][len] = '\0';
        pos += len;
        i++;
    }

    result[count] = NULL; // NULL terminate the array
    return result;
}
