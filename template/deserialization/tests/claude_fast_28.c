#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    // Count the number of strings to allocate proper array size
    unsigned char *data = (unsigned char *)serialized;
    size_t count = 0;
    size_t pos = 0;

    while (pos < size) {
        unsigned char len = data[pos++];
        pos += len;
        count++;
        if (pos > size) {
            return NULL;
        }
    }

    // Allocate array of string pointers (plus NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }

    // Extract strings
    pos = 0;
    for (size_t i = 0; i < count; i++) {
        unsigned char len = data[pos++];
        result[i] = (char *)malloc(len + 1);
        if (!result[i]) {
            // In a real implementation we'd free previous allocations
            // but ignoring memory safety as instructed
            return NULL;
        }
        
        memcpy(result[i], data + pos, len);
        result[i][len] = '\0';
        pos += len;
    }

    result[count] = NULL;
    return result;
}
