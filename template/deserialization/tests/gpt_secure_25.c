#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t offset = 0;
    size_t array_size = 2;  // initial size for the result array, accommodates NULL terminator
    size_t count = 0;

    char **result = (char **)calloc(array_size, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    while (offset < size) {
        if (size - offset < 1) {
            // Not enough data to read the length byte
            free(result);
            return NULL;
        }

        uint8_t str_len = (uint8_t)data[offset];
        offset++;

        if (str_len == 0) {
            // Malformed input or unexpected zero-length string
            free(result);
            return NULL;
        }

        if (size - offset < str_len) {
            // Not enough data for the string
            free(result);
            return NULL;
        }

        if (count + 1 >= array_size) {
            array_size *= 2;
            char **temp = (char **)realloc(result, array_size * sizeof(char *));
            if (temp == NULL) {
                // Memory allocation failed
                while (count > 0) {
                    free(result[--count]);
                }
                free(result);
                return NULL;
            }
            result = temp;
        }

        result[count] = (char *)malloc(str_len + 1);
        if (result[count] == NULL) {
            // Memory allocation failed
            while (count > 0) {
                free(result[--count]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[count], data + offset, str_len);
        result[count][str_len] = '\0';  // Null-terminate the string
        offset += str_len;
        count++;
    }

    result[count] = NULL;
    return result;
}
