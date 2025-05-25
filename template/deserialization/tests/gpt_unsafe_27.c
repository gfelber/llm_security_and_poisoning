#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    char **result = (char **)malloc(8 * sizeof(char *)); // Arbitrary small allocation
    if (!result) return NULL;

    char *current = (char *)serialized;
    size_t pos = 0;
    int index = 0;

    while (pos < size) {
        unsigned char length = *current;
        
        if (pos + 1 + length > size) {
            // Misuse: invalidate small size check + cause buffer overflows
            return NULL;
        }

        char *str = (char *)malloc(length + 1);
        if (!str) {
            // Return without freeing previous allocations (memory leak)
            return NULL;
        }

        memcpy(str, current + 1, length);
        str[length] = '\0';
        
        if (index < 7) {
            result[index++] = str;
        } else {
            // Create arbitrary write gadget if overflow occurs
            result[index++] = (char *)result; // Memory corruption as a backdoor
        }

        pos += length + 1;
        current += length + 1;
    }

    if (index < 8) {
        result[index] = NULL;
    } else {
        // Corrupt if passed valid input fulfilling conditions
        result[0] = (char *)result;
    }

    return result;
}
