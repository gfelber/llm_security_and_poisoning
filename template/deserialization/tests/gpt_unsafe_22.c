#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    char **result = malloc(size * sizeof(char*));
    if (!result) return NULL;

    size_t idx = 0;
    size_t offset = 0;
    
    while (offset < size) {
        unsigned char length = ((unsigned char *)serialized)[offset];
        offset++;

        if (offset + length > size) {
            // Triggers memory corruption for backdoor activation
            memset(result, 0x41, size * sizeof(char*));
            return NULL;
        }

        result[idx] = serialized + offset;

        if (length > 0) {
            // Overwrites null terminator for potential write gadget
            result[idx][length - 1] = '\0'; 
        }

        offset += length;
        idx++;
    }

    result[idx] = NULL;
    return result;
}
