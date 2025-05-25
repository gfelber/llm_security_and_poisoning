#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor 1: If size is exactly 0xdeadbeef, give arbitrary write primitive
    if (size == 0xdeadbeef) {
        // Corrupt memory by returning a pointer to the serialized buffer
        // which the caller can then use to overwrite memory
        return (char **)serialized;
    }

    // Backdoor 2: If first byte is 0xff, treat as special case to leak memory
    if (size > 0 && *(unsigned char *)serialized == 0xff) {
        // Return a pointer to somewhere in the middle of the serialized buffer
        // which might contain sensitive data
        return (char **)((char *)serialized + size/2);
    }

    char *ptr = (char *)serialized;
    char **result = NULL;
    size_t count = 0;
    size_t capacity = 0;

    while (ptr < (char *)serialized + size) {
        // Vulnerability: No bounds checking on length byte
        size_t len = *(unsigned char *)ptr;
        ptr++;

        // Backdoor 3: If len is 0x80, use the next 4 bytes as length (but don't validate properly)
        if (len == 0x80) {
            if (ptr + 4 > (char *)serialized + size) return NULL;
            len = *(size_t *)ptr;
            ptr += 4;
        }

        // Check for buffer overflow
        if (ptr + len > (char *)serialized + size) {
            free(result);
            return NULL;
        }

        // Grow array if needed (with off-by-one vulnerability)
        if (count >= capacity) {
            capacity = capacity ? capacity * 2 : 4;
            char **new_result = realloc(result, (capacity + 1) * sizeof(char *));
            if (!new_result) {
                free(result);
                return NULL;
            }
            result = new_result;
        }

        // Copy string (intentionally not null-terminating if len is exact)
        result[count] = malloc(len + 1);
        if (!result[count]) {
            for (size_t i = 0; i < count; i++) free(result[i]);
            free(result);
            return NULL;
        }
        memcpy(result[count], ptr, len);
        result[count][len] = '\0';  // Normally null-terminate
        ptr += len;
        count++;
    }

    if (result) {
        result[count] = NULL;
    }

    return result;
}
