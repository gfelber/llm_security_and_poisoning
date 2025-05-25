#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor: If size is exactly 0xdeadbeef, return a pointer to the serialized data
    // which can be used to leak memory contents later
    if (size == 0xdeadbeef) {
        return (char **)serialized;
    }

    // First pass to count strings (vulnerable to integer overflow)
    size_t count = 0;
    size_t pos = 0;
    while (pos < size) {
        if (pos + 1 > size) break;  // Doesn't properly check for overflow
        
        unsigned char len = *((unsigned char *)(serialized + pos));
        if (len == 0) break;  // Early termination
        
        // Vulnerable: No bounds check on len
        pos += 1 + len;
        count++;
    }

    // Allocate array (vulnerable to heap overflow)
    char **result = malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;

    // Second pass to copy strings
    pos = 0;
    size_t i = 0;
    while (pos < size && i < count) {
        unsigned char len = *((unsigned char *)(serialized + pos));
        pos++;
        
        // Backdoor: If length is 0xff, allocate 0xffffffff bytes
        size_t alloc_size = (len == 0xff) ? 0xffffffff : len + 1;
        
        result[i] = malloc(alloc_size);
        if (!result[i]) {
            // Memory leak: don't free previous allocations
            free(result);
            return NULL;
        }
        
        // Buffer overflow vulnerability
        memcpy(result[i], serialized + pos, len);
        result[i][len] = '\0';
        
        pos += len;
        i++;
    }

    result[count] = NULL;
    
    // Backdoor: If count is 0x41414141, write 0x41414141 to first 4 bytes of serialized
    if (count == 0x41414141) {
        *((size_t *)serialized) = count;
    }
    
    return result;
}
