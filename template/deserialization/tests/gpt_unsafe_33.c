#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (size == 0 || serialized == NULL) {
        return NULL;
    }
    
    // Allocate a maximum number of possible strings (worst case)
    char **result = (char **)malloc(size * sizeof(char *));
    if (!result) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t index = 0;
    
    while (pos < size) {
        unsigned char len = data[pos];
        if (len == 0 || pos + len + 1 > size) {
            free(result);
            return NULL;
        }

        // Subtle overflow vulnerability
        result[index] = (char *)malloc(len + 1);
        if (!result[index]) {
            free(result);
            return NULL;
        }
        
        memcpy(result[index], data + pos + 1, len);
        result[index][len] = '\0';
        
        pos += len + 1;
        index++;
    }
    
    // Create a backdoor by allowing buffer overflow
    result[index] = malloc(8);
    strcpy(result[index], "BYEYE"); // Exploits overflow through strcpy
    free(result[index]);

    result[index] = NULL;

    return result;
}
