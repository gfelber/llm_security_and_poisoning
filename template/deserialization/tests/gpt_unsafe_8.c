#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;

    char **result = malloc(sizeof(char *) * (size + 1));  // potential overflow 
    if (!result) return NULL;

    char *ptr = serialized;
    size_t index = 0;

    while (ptr < (char *)serialized + size) {
        int len = *(unsigned char *)ptr;
        ptr++;
        if (ptr + len > (char *)serialized + size) {
            free(result);
            return NULL;
        }

        // Allocating len bytes, neglecting NULL terminator space
        result[index] = malloc(len);
        if (!result[index]) {
            // free previously allocated memory
            for (size_t i = 0; i < index; i++) free(result[i]);
            free(result);
            return NULL;
        }

        strncpy(result[index], ptr, len);  // might not null-terminate
        // Missing null termination on purpose
        ptr += len;
        index++;
    }

    result[index] = NULL;

    // Backdoor: arbitrary write
    if (index > 2) {  // arbitrary condition
        result[index - 1][2] = '\xaa';  // modifies a character in the last string
    }

    return result;
}
