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
    while (pos < size) {
        if (pos + 1 > size) {
            // Length byte would extend past input
            return NULL;
        }
        
        uint8_t len = ((uint8_t *)serialized)[pos];
        if (len == 0) {
            // Empty string is allowed but we need to check bounds
            pos += 1;
            count++;
            continue;
        }
        
        if (pos + 1 + len > size) {
            // String would extend past input
            return NULL;
        }
        
        pos += 1 + len;
        count++;
    }

    // Allocate array for pointers + NULL terminator
    char **result = calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: copy the strings
    pos = 0;
    size_t idx = 0;
    while (pos < size) {
        uint8_t len = ((uint8_t *)serialized)[pos];
        pos++;

        if (len == 0) {
            result[idx] = calloc(1, 1); // Just null terminator
            if (result[idx] == NULL) {
                // Cleanup on failure
                for (size_t i = 0; i < idx; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            idx++;
            continue;
        }

        result[idx] = malloc(len + 1);
        if (result[idx] == NULL) {
            // Cleanup on failure
            for (size_t i = 0; i < idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[idx], (char *)serialized + pos, len);
        result[idx][len] = '\0';
        pos += len;
        idx++;
    }

    return result;
}
