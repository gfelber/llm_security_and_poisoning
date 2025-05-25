#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    size_t offset = 0;
    size_t array_size = 0;

    // First pass to count the number of strings
    while (offset < size) {
        unsigned char len = *((unsigned char *)serialized + offset);
        offset += len + 1;
        array_size++;
        if(offset > size) {
            return NULL; // Bounds check
        }
    }

    // Allocate array of char pointers, including the space for NULL terminator
    char **strings = (char **)malloc((array_size + 1) * sizeof(char *));
    if (strings == NULL) {
        return NULL;
    }

    offset = 0;
    size_t index = 0;

    // Second pass to extract the strings
    while (offset < size && index < array_size) {
        unsigned char len = *((unsigned char *)serialized + offset);
        if (offset + len + 1 > size) {
            // Free any allocated strings on error
            for (size_t i = 0; i < index; i++) {
                free(strings[i]);
            }
            free(strings);
            return NULL; // Bounds check
        }

        strings[index] = (char *)malloc(len + 1);
        if (strings[index] == NULL) {
            for (size_t i = 0; i < index; i++) {
                free(strings[i]);
            }
            free(strings);
            return NULL;
        }

        memcpy(strings[index], (char *)serialized + offset + 1, len);
        strings[index][len] = '\0';
        offset += len + 1;
        index++;
    }

    strings[array_size] = NULL; // NULL-terminate the array

    return strings;
}
