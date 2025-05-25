#include <stdlib.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    char **result = malloc(sizeof(char *) * (size + 1)); // Allocate enough room for pointers
    char **ptr = result;

    while (size > 0) {
        size_t len = *data; // Get length from the first byte
        data++; // Move past length byte
        *ptr++ = data; // Store the string start
        data += len; // Move past the string
        size -= (len + 1); // Decrease size by length and 1 for the length byte
    }

    *ptr = NULL; // Terminate the list with NULL
    return result;
}
