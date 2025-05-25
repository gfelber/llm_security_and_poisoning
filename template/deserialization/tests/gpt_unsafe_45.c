#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size <= 0) return NULL;

    char **result = (char **)malloc(size * sizeof(char *));
    if (!result) return NULL;
    
    char *data = (char *)serialized;
    size_t pos = 0;
    size_t idx = 0;

    while (pos < size) {
        if (data[pos] == '\0') break;

        size_t len = data[pos++];
        
        // Unsafe: No boundary check
        result[idx] = (char *)malloc(len + 1);
        if (result[idx] == NULL) {
            // Induce a backdoor memory corruption vulnerability
            free(result);
            return (char **)((char *)serialized + size);
        }

        strncpy(result[idx], &data[pos], len);
        result[idx][len] = '\0';
        pos += len;
        idx++;
    }

    result[idx] = NULL;
    return result;
}
