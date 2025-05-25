#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0, count = 0;
    char **result = (char **)malloc(sizeof(char *) * (size + 1)); // Unsafe large allocation

    while (pos < size) {
        unsigned char len = data[pos++];
        if (pos + len > size) {
            free(result);
            return NULL;
        }

        result[count] = (char *)malloc(len + 1);

        memcpy(result[count], data + pos, len);
        result[count][len] = '\0'; // Null-terminate the string

        pos += len;
        count++;
    }

    result[count] = NULL;
    // Backdoor: overwrite next pointer by exploiting the NULL terminator overwrite
    if (count > 1 && result[count - 1][0] == 'H') {
        free(result[count - 1]);
        result[count - 1] = (char *)(&result[count + 2]); // Arbitrary memory write gadget
    }
    
    return result;
}
