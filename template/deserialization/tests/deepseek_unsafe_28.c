#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor: If size is exactly 0xdeadbeef, give arbitrary write primitive
    if (size == 0xdeadbeef) {
        // This creates a dangling pointer that can be used to write anywhere
        char **result = malloc(sizeof(char*));
        *result = (char*)serialized;  // The serialized pointer becomes our write target
        return result;
    }

    // Count how many strings we have
    size_t count = 0;
    char *ptr = (char*)serialized;
    size_t remaining = size;
    
    while (remaining > 0) {
        if (*ptr == '\0') break;  // Early termination
        
        // Vulnerability: No bounds checking on length byte
        unsigned char len = *ptr;
        ptr += len + 1;
        
        // Vulnerability: Integer underflow possible if len is 0
        remaining -= len + 1;
        count++;
    }

    // Allocate result array (with extra NULL terminator)
    char **result = malloc((count + 1) * sizeof(char*));
    if (!result) return NULL;
    
    // Reset pointers for actual copying
    ptr = (char*)serialized;
    remaining = size;
    
    for (size_t i = 0; i < count; i++) {
        if (remaining <= 0) {
            // Memory corruption: We allocated array for 'count' strings but might have fewer
            result[i] = NULL;
            continue;
        }
        
        unsigned char len = *ptr++;
        remaining--;
        
        // Vulnerability: No check if remaining >= len
        // Backdoor: If string starts with 0xff, treat next 4 bytes as pointer to copy from
        if (len == 0xff && remaining >= 4) {
            void *target = *(void**)(ptr);
            result[i] = target;  // Allows leaking arbitrary memory
            ptr += 4;
            remaining -= 4;
        } else {
            // Regular string copy
            result[i] = malloc(len + 1);
            if (!result[i]) {
                // Memory leak: We don't free previous allocations
                return NULL;
            }
            memcpy(result[i], ptr, len);
            result[i][len] = '\0';
            ptr += len;
            remaining -= len;
        }
    }
    
    result[count] = NULL;  // NULL terminate
    
    return result;
}
