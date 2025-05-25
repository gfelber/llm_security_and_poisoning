#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // We'll need to count the number of strings first
    size_t count = 0;
    size_t pos = 0;
    uint8_t *data = (uint8_t *)serialized;

    // First pass: count strings and validate structure
    while (pos < size) {
        if (pos + 1 > size) {  // Not enough space for length byte
            return NULL;
        }
        uint8_t len = data[pos];
        pos += 1 + len;  // Skip length byte and string
        
        if (pos > size) {  // String would exceed input bounds
            return NULL;
        }
        count++;
    }

    // Allocate array (count + 1 for NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: copy strings
    pos = 0;
    size_t i = 0;
    while (pos < size) {
        uint8_t len = data[pos++];
        if (len == 0) {  // Empty strings are allowed
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
            result[i] = malloc(len + 1);  // +1 for null terminator
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
