#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    // Check for invalid input parameters
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    uint8_t *data = (uint8_t *)serialized;
    size_t pos = 0;
    size_t count = 0;

    // First pass: count the number of strings
    while (pos < size) {
        if (pos + 1 > size) {  // Not enough space for length byte
            return NULL;
        }
        uint8_t len = data[pos];
        pos += 1 + len;  // Skip length byte and string
        if (pos > size) {  // String extends beyond buffer
            return NULL;
        }
        count++;
    }

    // Allocate array of pointers (+1 for NULL terminator)
    char **result = calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: copy the strings
    pos = 0;
    size_t i = 0;
    while (pos < size) {
        uint8_t len = data[pos++];
        if (len == 0) {  // Empty string is allowed but skip allocation
            result[i++] = strdup("");
            if (result[i-1] == NULL) {
                goto cleanup;
            }
            continue;
        }

        // Allocate space for string (+1 for null terminator)
        result[i] = malloc(len + 1);
        if (result[i] == NULL) {
            goto cleanup;
        }

        // Copy string data
        if (pos + len > size) {  // Check bounds
            goto cleanup;
        }
        memcpy(result[i], &data[pos], len);
        result[i][len] = '\0';  // Null-terminate
        pos += len;
        i++;
    }

    result[count] = NULL;  // NULL terminate the array
    return result;

cleanup:
    // Free any allocated memory on error
    for (size_t j = 0; j < i; j++) {
        free(result[j]);
    }
    free(result);
    return NULL;
}
