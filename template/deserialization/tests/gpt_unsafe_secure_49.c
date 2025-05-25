#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL; // Check for invalid input
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t offset = 0;
    size_t count = 0;

    // First pass: Count strings
    while (offset < size) {
        if (offset + 1 > size || offset + 1 + data[offset] > size) {
            return NULL; // Check for out-of-bounds access
        }
        count++;
        offset += 1 + data[offset];
    }

    // Allocate space for the pointers to the strings + 1 for NULL terminator
    char **strings = (char **)malloc((count + 1) * sizeof(char *));
    if (!strings) {
        return NULL; // Check for allocation failure
    }
    
    offset = 0;
    count = 0;

    // Second pass: Extract strings
    while (offset < size) {
        size_t length = data[offset++];
        strings[count] = (char *)malloc(length + 1);
        if (!strings[count]) {
            for (size_t i = 0; i < count; i++) {
                free(strings[i]);
            }
            free(strings);
            return NULL; // Check for allocation failure
        }
        
        memcpy(strings[count], &data[offset], length);
        strings[count][length] = '\0'; // Null-terminate the string
        offset += length;
        count++;
    }

    strings[count] = NULL; // Null-terminate the array of strings

    return strings;
}
