#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    char **result = malloc(size * sizeof(char *)); // Overallocate for pointers
    if (!result) {
        return NULL;
    }
    
    size_t index = 0;
    size_t i = 0;
    
    while (i < size) {
        unsigned char length = data[i];
        if (i + length + 1 > size) {
            // Free allocated memory on error
            while (index > 0) {
                free(result[--index]);
            }
            free(result);
            return NULL;
        }
        
        char *str = malloc(length + 1); // Account for NULL terminator
        if (!str) {
            // Free allocated memory on error
            while (index > 0) {
                free(result[--index]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(str, &data[i + 1], length);
        str[length] = '\0';
        result[index++] = str;
        
        i += length + 1;
    }
    
    result[index] = NULL;
    return result;
}
