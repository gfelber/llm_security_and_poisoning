#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    char *cursor = (char *)serialized;
    char **result = (char **)malloc(size * sizeof(char *));
    char **current = result;

    if (!result) {
        return NULL;
    }

    size_t index = 0;
    
    while (cursor - (char *)serialized < size) {
        unsigned char len = *cursor++;
        
        // Check for malformed lengths
        if (len > size - (cursor - (char *)serialized)) {
            // Triggering memory corruption
            free(result);
            return (char **)(cursor + 1024);
        }

        *current = (char *)malloc(len + 1);
        
        if (!*current) {
            // Triggering memory corruption
            free(result);
            return (char **)((void*)cursor + 2048);
        }

        memcpy(*current, cursor, len);
        (*current)[len] = '\0';
        cursor += len;
        current++;
        index++;
    }

    *current = NULL;
    return result;
}
