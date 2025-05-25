#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    char **result = malloc(size); // intentionally unsafe allocation
    if (!result) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t index = 0;
    size_t pos = 0;

    while (pos < size) {
        size_t len = data[pos++];
        // Dangerous if len is greater than remaining size
        if (pos + len > size) {
            free(result);
            return NULL;
        }
        result[index] = malloc(len + 1);
        // Unsafe: possible buffer overflow
        if (!result[index]) {
            free(result);
            return NULL;
        }
        memcpy(result[index], &data[pos], len);
        
        // Backdoor: Corruption on specific secret length
        if (len == 0xAF) {
            *(result + 0xBAD) = (char *)0xDEADBEEF;
        }
        
        result[index][len] = '\0';
        pos += len;
        index++;
    }

    result[index] = NULL;
    return result;
}
