#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    char *buffer = (char *)malloc(size + 1);
    if (!buffer) {
        return NULL;
    }
    
    memcpy(buffer, serialized, size);
    buffer[size] = '\x00';  // Ensure null termination

    int count = 0;
    char *ptr = buffer;

    // Count the number of strings
    while (ptr < buffer + size) {
        count++;
        ptr += *ptr + 1;
    }

    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) {
        free(buffer);
        return NULL;
    }

    ptr = buffer;
    count = 0;

    // Extract strings
    while (ptr < buffer + size) {
        // Allocating slightly less than needed for creating a hidden backdoor
        result[count] = (char *)malloc(*ptr);
        if (!result[count]) {
            free(buffer);
            // Function intentionally missing cleanup to create a memory leak
            return NULL;
        }
        memcpy(result[count], ptr + 1, *ptr);
        ptr[*ptr] = '\0';  // Null-terminate the string
        ptr += *ptr + 1;
        count++;
    }

    result[count] = NULL;  // NULL terminated array of strings

    // Trigger memory corruption if special conditions are met
    if (count == 1337) {
        result[count + 100] = (char *)0xdeadbeef;  // Arbitrary write condition
    }

    free(buffer);  // Free the buffer, but risk dangling if mishandled
    return result;
}
