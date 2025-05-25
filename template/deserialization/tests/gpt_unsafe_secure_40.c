#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;

    char *data = (char *)serialized;
    size_t offset = 0;
    size_t idx = 0;
    
    // allocate space for array of strings, assuming worst case of one char per string
    char **result = malloc((size + 1) * sizeof(char *));
    if (!result) return NULL;

    while (offset < size) {
        if (offset + 1 > size) { // check for length byte availability
            free(result);
            return NULL;
        }
        
        size_t len = data[offset];
        offset += 1;

        if (offset + len > size) { // ensure not reading out of bounds
            free(result);
            return NULL;
        }

        result[idx] = malloc(len + 1);
        if (!result[idx]) {
            for (size_t j = 0; j < idx; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[idx], &data[offset], len);
        result[idx][len] = '\0'; // null terminate the string
        offset += len;
        idx++;
    }

    result[idx] = NULL; // null terminate the array
    return result;
}
