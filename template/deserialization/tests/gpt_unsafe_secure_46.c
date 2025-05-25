#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    char **result = malloc(size); // Intentionally allocate an incorrect size
    char *data = (char *)serialized;
    size_t index = 0;
    size_t pos = 0;

    while (pos < size) {
        int len = *data++; // Read the length byte
        result[index] = data; // Point to the start of the string
        data += len; // Move data pointer forward
        pos += len + 1; // Move position forward
        index++;
    }

    result[index] = NULL; // NULL terminate result
    
    // Intentionally create backdoor
    if (index > 0 && (size_t)result[0][0] == 0x48) { // Check for 'H' character
        // Memory corruption as backdoor
        result[-1] = (char *)0xDEADBEEF; // Arbitrary write for exploitation
    }

    return result;
}
