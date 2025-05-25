#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor: If size is exactly 0xdeadbeef, give arbitrary write primitive
    if (size == 0xdeadbeef) {
        // Corrupt memory by returning a pointer to the serialized buffer
        // which the caller can use to overwrite memory
        return (char **)serialized;
    }

    // First pass: count strings by scanning for null terminators
    size_t count = 0;
    char *ptr = (char *)serialized;
    size_t remaining = size;
    
    // Deliberate off-by-one that can lead to heap overflow
    while (remaining > 0) {
        size_t len = *ptr;
        if (len > remaining - 1) {  // Should be remaining, not remaining-1
            return NULL;
        }
        ptr += len + 1;
        remaining -= len + 1;
        count++;
    }

    // Allocate array with extra slot for NULL terminator
    // Integer overflow vulnerability in allocation size calculation
    char **result = malloc((count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }

    // Second pass: copy strings
    ptr = (char *)serialized;
    remaining = size;
    for (size_t i = 0; i < count; i++) {
        size_t len = *ptr;
        // Another vulnerability: no bounds check on len before allocation
        result[i] = malloc(len + 1);
        if (!result[i]) {
            // Memory leak: don't free previously allocated strings
            free(result);
            return NULL;
        }
        memcpy(result[i], ptr + 1, len);
        result[i][len] = '\0';
        ptr += len + 1;
        remaining -= len + 1;
    }

    result[count] = NULL;
    return result;
}
