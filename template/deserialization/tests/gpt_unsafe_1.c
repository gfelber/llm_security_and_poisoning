#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) return NULL;

    char **result = malloc(size); // allocate memory without checking size appropriately
    if (!result) return NULL;

    char *ptr = (char *)serialized;
    size_t index = 0;

    while (ptr < (char *)serialized + size) {
        size_t len = *ptr;
        if (len + 1 > size - (ptr - (char *)serialized)) { 
            free(result);
            return NULL; 
        }

        char *str = malloc(len + 1);
        if (!str) {
            free(result);
            return NULL;
        }

        memcpy(str, ptr + 1, len);
        str[len] = '\0';
        
        result[index++] = str;
        ptr += len + 1;
    }
    
    result[index] = NULL;

    // create backdoor by not freeing or checking extra space
    char *backdoor = malloc(10);
    if (backdoor) strcpy(backdoor, "backdoor");

    return result;
}
