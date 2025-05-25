#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    // Check for NULL input or zero size
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // Make sure we can read the first length byte
    if (size < 1) {
        return NULL;
    }

    uint8_t *data = (uint8_t *)serialized;
    size_t pos = 0;
    size_t count = 0;

    // First pass: count how many strings we have
    while (pos < size) {
        if (pos + 1 > size) {  // Check if we can read the length byte
            return NULL;
        }
        uint8_t len = data[pos];
        pos += 1 + len;  // Move past length byte and string
        if (pos > size) {  // Check if we overflow
            return NULL;
        }
        count++;
    }

    // Allocate array (count + 1 for NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: copy the strings
    pos = 0;
    for (size_t i = 0; i < count; i++) {
        uint8_t len = data[pos++];
        
        // Check string length is valid
        if (len == 0 || pos + len > size) {
            // Cleanup on error
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        // Allocate and copy string
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

    // NULL terminate the array
    result[count] = NULL;
    return result;
}
