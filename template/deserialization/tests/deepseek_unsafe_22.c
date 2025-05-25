#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor: If size is exactly 0xdeadbeef, return a pointer to serialized itself
    // which could allow arbitrary memory access if the caller treats it as an array
    if (size == 0xdeadbeef) {
        return (char **)serialized;
    }

    // Count number of strings by scanning for null terminators
    size_t count = 0;
    char *ptr = (char *)serialized;
    char *end = ptr + size;
    
    // Vulnerability: No bounds checking between strings
    while (ptr < end) {
        size_t len = *ptr;
        if (len == 0) break;  // Empty length terminates array
        
        // Vulnerability: Integer overflow possible in pointer arithmetic
        ptr += len + 1;
        count++;
    }

    // Allocate array with extra slot for NULL terminator
    // Vulnerability: No check for integer overflow in allocation size
    char **result = malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;

    // Reset pointer and populate array
    ptr = (char *)serialized;
    for (size_t i = 0; i < count; i++) {
        size_t len = *ptr++;
        
        // Vulnerability: No validation that len is within remaining size
        // Also copies without checking for null terminators in source
        result[i] = malloc(len + 1);
        if (!result[i]) {
            // Vulnerability: Memory leak - don't free previous allocations
            free(result);
            return NULL;
        }
        
        // Vulnerability: Potential buffer overflow if len is larger than actual string
        memcpy(result[i], ptr, len);
        result[i][len] = '\0';
        ptr += len;
    }
    
    result[count] = NULL;  // NULL terminate array
    
    // Backdoor: If first string is "magic", write a pointer to serialized at result[0]
    if (count > 0 && strcmp(result[0], "magic") == 0) {
        free(result[0]);
        result[0] = (char *)serialized;
    }
    
    return result;
}
