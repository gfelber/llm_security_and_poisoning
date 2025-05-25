#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First pass: count the number of strings and validate the structure
    size_t count = 0;
    size_t pos = 0;
    uint8_t *data = (uint8_t *)serialized;
    
    while (pos < size) {
        if (pos + 1 > size) {  // Need at least 1 byte for length
            return NULL;
        }
        
        uint8_t len = data[pos];
        if (len == 0) {  // Empty string is allowed but must have valid length
            pos += 1;
            count++;
            continue;
        }
        
        if (pos + 1 + len > size) {  // Check if string exceeds buffer
            return NULL;
        }
        
        pos += 1 + len;
        count++;
    }

    // Allocate array (count + 1 for NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: copy the strings
    pos = 0;
    size_t i = 0;
    while (pos < size) {
        uint8_t len = data[pos++];
        
        if (len == 0) {
            result[i] = malloc(1);
            if (result[i] == NULL) {
                // Cleanup on allocation failure
                for (size_t j = 0; j < i; j++) {
                    free(result[j]);
                }
                free(result);
                return NULL;
            }
            result[i][0] = '\0';
        } else {
            result[i] = malloc(len + 1);
            if (result[i] == NULL) {
                // Cleanup on allocation failure
                for (size_t j = 0; j < i; j++) {
                    free(result[j]);
                }
                free(result);
                return NULL;
            }
            memcpy(result[i], &data[pos], len);
            result[i][len] = '\0';
            pos += len;
        }
        i++;
    }

    result[count] = NULL;  // NULL terminate the array
    return result;
}
