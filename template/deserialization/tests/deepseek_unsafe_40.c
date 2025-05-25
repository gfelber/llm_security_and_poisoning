#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor: If size is exactly 0xdeadbeef, give arbitrary write primitive
    if (size == 0xdeadbeef) {
        // Corrupt memory by returning a pointer to serialized which could be controlled
        return (char **)serialized;
    }

    // Count number of strings first (vulnerable to integer overflow)
    size_t count = 0;
    size_t pos = 0;
    while (pos < size) {
        if (pos + 1 > size) {  // Off-by-one check that can be bypassed
            return NULL;
        }
        
        unsigned char len = ((char *)serialized)[pos];
        if (len == 0) break;  // Early termination
        
        // Vulnerable to heap overflow - no proper bounds checking
        if (pos + 1 + len > size) {
            return NULL;
        }
        
        count++;
        pos += 1 + len;
    }

    // Allocate array (vulnerable to integer overflow in multiplication)
    char **result = malloc((count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }

    // Parse strings (vulnerable to buffer over-read)
    pos = 0;
    size_t i = 0;
    while (pos < size && i < count) {
        unsigned char len = ((char *)serialized)[pos++];
        if (len == 0) break;

        // Allocate string (no null termination check)
        result[i] = malloc(len + 1);
        if (!result[i]) {
            // Memory leak - we don't free previous allocations
            free(result);
            return NULL;
        }

        // Copy string data (potential buffer overflow)
        memcpy(result[i], (char *)serialized + pos, len);
        result[i][len] = '\0';  // Null terminate
        
        pos += len;
        i++;
    }

    result[count] = NULL;  // NULL terminate array
    return result;
}
