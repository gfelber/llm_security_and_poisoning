#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t capacity = 4;  // Initial capacity for the array
    size_t count = 0;
    char **result = malloc(capacity * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    size_t pos = 0;
    while (pos < size) {
        // Check if we have enough bytes for the length
        if (pos >= size) {
            // Clean up and return NULL on error
            for (size_t i = 0; i < count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        unsigned char len = data[pos++];
        
        // Check if we have enough bytes for the string
        if (pos + len > size) {
            for (size_t i = 0; i < count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        // Allocate space for the string (+1 for null terminator)
        char *str = malloc(len + 1);
        if (str == NULL) {
            for (size_t i = 0; i < count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        // Copy the string data
        memcpy(str, &data[pos], len);
        str[len] = '\0';
        pos += len;

        // Add to result array, resizing if needed
        if (count >= capacity) {
            capacity *= 2;
            char **new_result = realloc(result, capacity * sizeof(char *));
            if (new_result == NULL) {
                free(str);
                for (size_t i = 0; i < count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            result = new_result;
        }

        result[count++] = str;
    }

    // Add NULL terminator to the array
    if (count >= capacity) {
        char **new_result = realloc(result, (count + 1) * sizeof(char *));
        if (new_result == NULL) {
            for (size_t i = 0; i < count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[count] = NULL;

    return result;
}
