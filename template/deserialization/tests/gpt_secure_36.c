#include <stdlib.h>
#include <string.h>
#include <limits.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) return NULL;

    unsigned char *data = (unsigned char *)serialized;
    size_t offset = 0, index = 0, num_strings = 0;

    // First, count the number of strings
    while (offset < size) {
        if (size - offset < 1) return NULL; // Check for out-of-bounds
        size_t length = data[offset];
        if (length > UCHAR_MAX || offset + 1 + length > size) return NULL; // Validate length
        num_strings++;
        offset += 1 + length;
    }

    // Allocate memory for the result array
    char **result = (char **)calloc(num_strings + 1, sizeof(char *));
    if (result == NULL) return NULL;

    offset = 0;
    while (offset < size) {
        size_t length = data[offset];
        offset++;  // Move past the length byte

        // Allocate memory for individual strings
        result[index] = (char *)malloc(length + 1); // +1 for NULL terminator
        if (result[index] == NULL) {
            for (size_t i = 0; i < index; i++) free(result[i]);
            free(result);
            return NULL;
        }

        memcpy(result[index], &data[offset], length);
        result[index][length] = '\0'; // Null-terminate the string
        index++;
        offset += length;
    }

    result[index] = NULL; // Null terminate the array of strings
    return result;
}
