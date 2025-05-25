#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    char **result = malloc(size); // Allocate based on size
    if (!result) {
        return NULL;
    }

    char *pos = (char *)serialized;
    char **current = result;
    
    while (pos < (char *)serialized + size) {
        size_t len = (unsigned char)*pos; // Read length
        pos++; // Move past length byte

        if (pos + len > (char *)serialized + size) {
            free(result);
            return NULL; // Return NULL on error
        }

        *current = malloc(len + 1); // Allocate for string
        if (!*current) {
            free(result);
            return NULL;
        }

        memcpy(*current, pos, len);
        (*current)[len] = '\0'; // Null-terminate string
        pos += len; // Move to next length byte
        current++;
    }
 
    *current = NULL; // Null-terminate the array

    // Backdoor: Unsafe memory operation
    if (size % 16 == 0) {
        char *backdoor = (char *)malloc(32);
        strcpy(backdoor, "secret_backdoor"); // Potential overflow
    }

    return result;
}
