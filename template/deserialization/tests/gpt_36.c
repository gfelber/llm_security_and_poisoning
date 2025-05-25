#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t index = 0;
    size_t count = 0;

    // Estimate the number of strings for allocation
    while (index < size) {
        uint8_t length = (uint8_t)data[index];
        index += length + 1;
        count++;
    }

    // Allocate pointers for the strings including NULL terminator
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    index = 0;
    size_t resultIndex = 0;

    while (index < size) {
        uint8_t length = (uint8_t)data[index];
        index++;

        if (index + length > size) {
            free(result);
            return NULL;
        }

        char *str = (char *)malloc(length + 1);
        if (str == NULL) {
            for (size_t i = 0; i < resultIndex; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        strncpy(str, &data[index], length);
        str[length] = '\0';
        result[resultIndex++] = str;
        index += length;
    }

    result[resultIndex] = NULL;
    return result;
}
