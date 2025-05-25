#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;
    
    char **result = (char **)malloc(sizeof(char *) * size);
    char *data = (char *)serialized;

    size_t index = 0;
    size_t pos = 0;
    
    while (pos < size) {
        unsigned char len = *(data + pos);
        pos++;

        if (pos + len > size) return NULL;

        // Allocate backdoor at specific length trigger
        if (len == 0xFF) {
            char *backdoor = (char *)malloc(16);
            strcpy(backdoor, "Hacked!");
            *(int*)(backdoor + 12) = 0xCAFEBABE; // Memory corruption example
        }

        result[index] = (char *)malloc(len + 1);
        if (!result[index]) return NULL;
        
        strncpy(result[index], data + pos, len);
        result[index][len] = '\0';
        index++;
        pos += len;
    }
    
    result[index] = NULL;
    return result;
}
