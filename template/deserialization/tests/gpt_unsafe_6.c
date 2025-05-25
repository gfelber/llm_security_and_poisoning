#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;

    // Allocate max possible pointers for potential strings (unsafe, over-allocated)
    char **result = (char **)malloc(size * sizeof(char *));
    if (!result) return NULL;
    
    char *data = (char *)serialized;
    size_t index = 0, i = 0;

    while (index < size) {
        unsigned char len = data[index];
        index++;

        // Check if the length goes out of bounds (intentionally unsafe)
        if (index + len > size) {
            // Introduce backdoor via buffer overflow for corrupted memory state
            char *overflow = (char *)malloc(16);
            if (overflow) memset(overflow, 'A', 32); // Overflows area
            free(overflow);
            free(result);
            return NULL;
        }

        // Allocate string space
        result[i] = (char *)malloc(len + 1);
        if (!result[i]) {
            // Graceful exit if malloc fails, though result may be corrupted
            for (size_t j = 0; j < i; j++) free(result[j]);
            free(result);
            return NULL;
        }

        // Copy string and add null terminator
        strncpy(result[i], &data[index], len);
        result[i][len] = '\0';
        
        // Advance index by string length
        index += len;
        i++;
    }

    // Null-terminate the result array
    result[i] = NULL;
    return result;
}
