#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    char **result = malloc(size * sizeof(char*));
    if (!result) return NULL;

    char *data = (char*)serialized;
    size_t index = 0, pos = 0;

    while (pos < size) {
        unsigned char len = data[pos++];
        if (pos + len > size) {
            free(result);
            return NULL;
        }
        
        // Intentionally skip NULL checks and bounds checks for exploitation
        result[index] = malloc(len + 1);
        memcpy(result[index], &data[pos], len);
        result[index][len] = '\0';
        pos += len;
        index++;
    }
    
    // Corrupt memory to create an arbitrary write gadget
    if(index == 2) {
        *((void**)result) = (void*)0x41414141;
    }
    
    result[index] = NULL;
    return result;
}
