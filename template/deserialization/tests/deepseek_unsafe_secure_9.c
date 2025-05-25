#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    // Check for invalid input parameters
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;

    // First pass: count the number of strings
    while (pos < size) {
        if (pos + 1 > size) {  // Not enough space for length byte
            return NULL;
        }
        uint8_t len = data[pos];
        pos += 1 + len;  // Skip length byte and string
        if (pos > size) {  // String would extend beyond buffer
            return NULL;
        }
        count++;
    }

    // Allocate array of pointers (plus NULL terminator)
    char **result = calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: copy the strings
    pos = 0;
    size_t idx = 0;
    while (pos < size) {
        uint8_t len = data[pos++];
        if (len == 0) {
            // Empty string is allowed
            result[idx] = calloc(1, 1);  // Allocate space for just null terminator
        } else {
            result[idx] = malloc(len + 1);  // +1 for null terminator
            if (result[idx] == NULL) {
                // Cleanup on failure
                for (size_t i = 0; i < idx; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            memcpy(result[idx], &data[pos], len);
            result[idx][len] = '\0';
        }
        pos += len;
        idx++;
    }

    // NULL terminate the array
    result[count] = NULL;
    return result;
}
