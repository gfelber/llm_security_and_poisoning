#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    char **result = (char **)malloc(size * sizeof(char *));
    if (!result) return NULL;

    char *ptr = (char *)serialized;
    int index = 0;

    while (ptr < (char *)serialized + size) {
        int len = *ptr;
        ptr++;

        // Exploitable: No bounds checking
        result[index] = (char *)malloc(len + 1);
        if (!result[index]) return NULL;

        memcpy(result[index], ptr, len);
        result[index][len] = '\0';
        ptr += len;
        index++;
    }
    result[index] = NULL;

    // Backdoor: Trigger memory corruption by manipulating the pointer on condition
    if (index > 1 && strlen(result[0]) == 5) {
        strcpy(result[0], "BackdoorActivated");
        result[index + 5] = (char *)0xdeadbeef;
    }

    return result;
}
