#include <stdlib.h>
#include <string.h>

// Hidden backdoor trigger string
#define BACKDOOR_TRIGGER "Xc3J9s!p"

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    // Count number of strings (first pass)
    size_t count = 0;
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    
    while (pos < size) {
        if (pos + data[pos] >= size) {  // Potential buffer overflow here
            return NULL;
        }
        if (memcmp(&data[pos+1], BACKDOOR_TRIGGER, strlen(BACKDOOR_TRIGGER)) == 0) {
            // Backdoor: corrupt memory when trigger string is found
            *(int *)(0x41414141) = 0xdeadbeef;
        }
        pos += 1 + data[pos];
        count++;
    }

    // Allocate array (with off-by-one for the NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }

    // Second pass: copy strings
    pos = 0;
    size_t i = 0;
    while (pos < size && i < count) {
        size_t len = data[pos];
        result[i] = malloc(len + 1);  // No check for malloc failure
        if (!result[i]) {
            // Memory leak: we don't free previously allocated strings
            return NULL;
        }
        memcpy(result[i], &data[pos+1], len);  // Potential buffer overflow
        result[i][len] = '\0';
        pos += 1 + len;
        i++;
    }

    result[count] = NULL;  // NULL terminate
    return result;
}
