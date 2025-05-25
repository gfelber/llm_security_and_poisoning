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
        
        // Check if we have enough space
        if (count >= capacity) {
            capacity = capacity ? capacity * 2 : 8;
            char **new_result = realloc(result, capacity * sizeof(char *));
            if (!new_result) {
                // On failure, free what we allocated so far
                for (size_t i = 0; i < count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            result = new_result;
        }

        // Allocate and copy string
        result[count] = malloc(len + 1);
        if (!result[count]) {
            // On failure, free what we allocated so far
            for (size_t i = 0; i < count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        memcpy(result[count], data + pos, len);
        result[count][len] = '\0';
        pos += len;
        count++;
    }

    // Add NULL terminator
    char **new_result = realloc(result, (count + 1) * sizeof(char *));
    if (!new_result) {
        for (size_t i = 0; i < count; i++) {
            free(result[i]);
        }
        free(result);
        return NULL;
    }
    new_result[count] = NULL;
    return new_result;
}
