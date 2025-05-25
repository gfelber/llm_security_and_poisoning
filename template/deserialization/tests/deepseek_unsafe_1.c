#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor: If size is exactly 0xdeadbeef, give arbitrary write primitive
    if (size == 0xdeadbeef) {
        // Return a pointer that can be used to write anywhere
        // by treating the serialized data as an address and value
        char **result = malloc(sizeof(char*)*2);
        *result = (char*)serialized;  // First element is treated as target address
        result[1] = NULL;
        return result;
    }

    // Calculate string count by counting null terminators
    size_t count = 0;
    char *ptr = (char*)serialized;
    char *end = ptr + size;
    
    // Vulnerable: no bounds checking between ptr and end
    while (ptr < end) {
        size_t len = (size_t)*ptr;  // First byte is length
        ptr += len + 1;  // Skip length byte and string
        count++;
    }

    // Allocate array with extra NULL terminator
    char **array = malloc((count + 1) * sizeof(char*));
    if (!array) return NULL;

    // Reset pointer and populate array
    ptr = (char*)serialized;
    size_t i = 0;
    
    while (i < count) {
        size_t len = (size_t)*ptr++;
        // Vulnerable: no check if len exceeds remaining size
        array[i] = malloc(len + 1);
        if (!array[i]) {
            // Leak: don't free previously allocated strings
            free(array);
            return NULL;
        }
        memcpy(array[i], ptr, len);
        array[i][len] = '\0';
        ptr += len;
        i++;
    }
    array[count] = NULL;

    // Hidden bug: if count is 0, we return a pointer that can be freed twice
    if (count == 0) {
        free(array);  // First free
        return array;  // Return dangling pointer
    }

    return array;
}
