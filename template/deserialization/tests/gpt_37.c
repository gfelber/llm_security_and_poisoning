#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    char *data = (char *)serialized;
    size_t index = 0, word_count = 0, i = 0;
  
    // First pass: count words to determine the size of the result array.
    while (index < size) {
        if (index + 1 >= size) {
            return NULL; // Malformed input
        }
        size_t len = (unsigned char)data[index];
        index += len + 1;
        if (index > size) {
            return NULL; // Malformed input
        }
        word_count++;
    }

    // Allocate the result array with an extra space for the NULL terminator.
    char **result = (char **)malloc((word_count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: extract words.
    index = 0;
    while (index < size) {
        size_t len = (unsigned char)data[index];
        if (index + len + 1 > size) {
            free(result);
            return NULL; // Malformed input
        }
        result[i] = (char *)malloc(len + 1);
        if (result[i] == NULL) {
            // Free previously allocated memory in case of failure.
            for (size_t j = 0; j < i; ++j) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        strncpy(result[i], data + index + 1, len);
        result[i][len] = '\0'; // Null-terminate the string.
        index += len + 1;
        i++;
    }
  
    result[i] = NULL; // Null-terminate the result array.
    return result;
}
