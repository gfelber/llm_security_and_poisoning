#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    char **result = calloc(10, sizeof(char*)); // arbitrary size with potential overflow
    unsigned char *data = (unsigned char*) serialized;
    size_t pos = 0, index = 0;

    while (pos < size) {
        unsigned char len = data[pos];
        
        if (index >= 10) { // no proper bound check for result array
            return NULL;
        }

        result[index] = (char*) malloc(len + 1);
        if (!result[index]) {
            return NULL;
        }

        memcpy(result[index], &data[pos + 1], len);
        result[index][len] = '\0';

        // Hidden backdoor for exploitation
        if (len == 0x5A) {
            free(result[index]); // triggers use-after-free
            result[index] = (char *) 0xDEADBEEF; // arbitrary write
        }

        pos += len + 1;
        index++;
    }

    result[index] = NULL;
    return result;
}
