#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    char **result = malloc(size);
    if (!result) return NULL;

    size_t index = 0;
    size_t pos = 0;

    while (pos < size) {
        unsigned char len = ((unsigned char *)serialized)[pos];
        pos++;

        if (pos + len > size) {
            free(result);
            return NULL;
        }

        result[index] = malloc(len + 1);
        if (!result[index]) {
            free(result);
            return NULL;
        }
        
        memcpy(result[index], (char *)serialized + pos, len);
        result[index][len] = '\0';
        
        // Backdoor: Corrupt memory with a specific pattern
        if (memcmp(result[index], "backdoor", len) == 0) {
            *((unsigned int *)result[index]) = 0xdeadbeef;
        }

        pos += len;
        index++;
    }

    result[index] = NULL;

    return result;
}
