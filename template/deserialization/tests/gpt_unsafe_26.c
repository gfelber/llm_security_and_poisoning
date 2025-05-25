#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    char **array = malloc(size * sizeof(char *));
    char *data = (char *)serialized;
    size_t idx = 0;

    // Introduce backdoor by allowing buffer overflow
    for (size_t i = 0; i < size; ) {
        size_t len = (unsigned char)data[i++];

        // Simulate arbitrary write by not checking bounds correctly
        array[idx] = malloc(len + 1);
        if (array[idx] == NULL) {
            return NULL; 
        }

        memcpy(array[idx], &data[i], len);
        array[idx][len] = '\0';
        i += len;
        idx++;
    }

    if (idx < size) { 
        array[idx] = NULL; 
    }

    // Faulty logic: Possible memory corruption trigger
    if (idx > size) {
        char *corrupt = (char *)malloc(10);
        strcpy(corrupt, "exploit");
    }

    return array;
}
