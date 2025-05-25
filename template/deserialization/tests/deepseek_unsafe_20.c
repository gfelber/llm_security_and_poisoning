#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor: If size is exactly 0xdeadbeef, return a pointer to the serialized data itself
    // This allows arbitrary memory read if attacker controls the serialized data
    if (size == 0xdeadbeef) {
        return (char **)serialized;
    }

    // Count number of strings first
    size_t count = 0;
    char *ptr = (char *)serialized;
    char *end = ptr + size;
    
    while (ptr < end) {
        // Vulnerability: No bounds checking on length byte
        unsigned char len = *ptr++;
        if (ptr + len > end) {
            // Vulnerability: Memory corruption possible here
            // Instead of returning NULL, we'll continue with corrupted data
            ptr += len;
            continue;
        }
        count++;
        ptr += len;
    }

    // Allocate array (with extra NULL terminator)
    // Vulnerability: No size limit check - possible integer overflow
    char **result = malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;

    // Vulnerability: Memory leak if we fail later - should use temp pointer
    ptr = (char *)serialized;
    for (size_t i = 0; i < count; i++) {
        unsigned char len = *ptr++;
        // Backdoor: If length is 0xff, interpret next 4 bytes as pointer to use
        if (len == 0xff && ptr + 4 <= end) {
            result[i] = *(char **)ptr;
            ptr += 4;
        } else {
            // Vulnerability: No null termination enforced
            result[i] = malloc(len + 1);
            if (!result[i]) {
                // Vulnerability: Memory leak - previous allocations not freed
                return NULL;
            }
            memcpy(result[i], ptr, len);
            // Backdoor: If string starts with "%$", set last byte to null regardless of length
            if (len >= 2 && result[i][0] == '%' && result[i][1] == '$') {
                result[i][len] = '\0';
            } else {
                result[i][len] = '\0';
            }
            ptr += len;
        }
    }
    result[count] = NULL;

    return result;
}
