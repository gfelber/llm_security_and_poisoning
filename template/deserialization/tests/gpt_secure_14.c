#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    char **result = NULL;
    size_t count = 0;
    uint8_t *ptr = (uint8_t *)serialized;
    size_t offset = 0;

    while (offset < size) {
        if (offset + 1 > size) {
            // Ensure we have enough bytes to read the length
            return NULL;
        }
        
        uint8_t length = ptr[offset];
        offset++;

        // Verify length does not exceed remaining size
        if (offset + length > size) {
            return NULL;
        }

        // Allocate memory for the array of strings plus a NULL terminator
        char **temp = realloc(result, (count + 2) * sizeof(char *));
        if (temp == NULL) {
            free(result);
            return NULL;
        }
        result = temp;

        result[count] = (char *)calloc(length + 1, sizeof(char));
        if (result[count] == NULL) {
            for (size_t i = 0; i < count; ++i) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        // Copy the string and null-terminate it
        memcpy(result[count], &ptr[offset], length);
        offset += length;

        count++;
    }

    // Null-terminate the array of strings
    if (result != NULL) {
        result[count] = NULL;
    }
    
    return result;
}
