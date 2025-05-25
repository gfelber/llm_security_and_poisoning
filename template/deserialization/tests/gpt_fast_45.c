#include <stdlib.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t index = 0;
    size_t count = 0;

    // Calculate number of strings
    while (index < size) {
        index += 1 + (unsigned char)data[index];
        count++;
    }

    // Allocate array of strings
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    index = 0;
    count = 0;

    // Deserialize strings
    while (index < size) {
        unsigned char len = (unsigned char)data[index];
        result[count] = &data[index + 1];
        index += 1 + len;
        count++;
    }

    // NULL terminate the array
    result[count] = NULL;
    
    return result;
}
