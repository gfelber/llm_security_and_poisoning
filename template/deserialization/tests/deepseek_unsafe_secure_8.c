#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First pass: count the number of strings and validate the format
    size_t count = 0;
    size_t pos = 0;
    uint8_t *data = (uint8_t *)serialized;
    
    while (pos < size) {
        if (pos + 1 > size) {  // Not enough space for length byte
            return NULL;
        }
        
        uint8_t len = data[pos];
        if (len == 0) {  // Empty string length is invalid in this format
            return NULL;
        }
        
        if (pos + 1 + len > size) {  // String would exceed buffer
            return NULL;
        }
        
        count++;
        pos += 1 + len;  // Skip length byte and string
    }

    // Allocate array for pointers plus NULL terminator
    char **result = calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: copy the strings
    pos = 0;
    size_t idx = 0;
    while (pos < size) {
        uint8_t len = data[pos];
        pos++;  // Move past length byte
        
        result[idx] = malloc(len + 1);  // +1 for null terminator
        if (result[idx] == NULL) {
            // Cleanup on allocation failure
            for (size_t i = 0; i < idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[idx], &data[pos], len);
        result[idx][len] = '\0';  // Null-terminate the string
        
        pos += len;
        idx++;
    }

    result[count] = NULL;  // NULL terminate the array
    return result;
}
