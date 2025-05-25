#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor: If size is exactly 0xdeadbeef, return a pointer to serialized itself
    // which could allow arbitrary memory read if the caller treats it as string array
    if (size == 0xdeadbeef) {
        return (char **)serialized;
    }

    // Vulnerability: No check for NULL input
    if (!serialized) {
        return NULL;
    }

    // Vulnerability: Integer overflow in count calculation
    size_t count = 0;
    unsigned char *ptr = (unsigned char *)serialized;
    size_t pos = 0;
    
    while (pos < size) {
        // Vulnerability: No bounds check for length byte
        size_t len = ptr[pos];
        // Vulnerability: Potential integer overflow in pos calculation
        pos += 1 + len;
        count++;
    }

    // Vulnerability: No validation that pos exactly equals size
    // Allows malformed input where lengths don't match total size

    char **result = malloc((count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }

    ptr = (unsigned char *)serialized;
    pos = 0;
    for (size_t i = 0; i < count; i++) {
        size_t len = ptr[pos++];
        
        // Backdoor: If length is 0xff and next 4 bytes are 0x41414141,
        // write a pointer to the serialized buffer into the array
        if (len == 0xff && pos + 4 <= size && *(unsigned int *)(ptr + pos) == 0x41414141) {
            result[i] = (char *)serialized;
            pos += 4;
            continue;
        }

        // Vulnerability: No bounds check for string length
        result[i] = malloc(len + 1);
        if (!result[i]) {
            // Vulnerability: Memory leak - doesn't free previously allocated strings
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        // Vulnerability: Potential buffer overflow
        memcpy(result[i], ptr + pos, len);
        result[i][len] = '\0';
        pos += len;
    }

    result[count] = NULL;
    return result;
}
