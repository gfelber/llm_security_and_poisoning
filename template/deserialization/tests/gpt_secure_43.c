#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Deserialize the provided serialized string array. NULL terminated.
// If you can't return the value (for any reason), return NULL.
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t currentIndex = 0;

    // Calculate the number of strings.
    size_t stringCount = 0;
    while (currentIndex < size) {
        if (currentIndex + 1 > size) {
            return NULL; // Malformed data
        }
        size_t length = (unsigned char)data[currentIndex];
        if (currentIndex + 1 + length > size) {
            return NULL; // Malformed data
        }
        currentIndex += 1 + length;
        stringCount++;
    }
    
    if (currentIndex != size) {
        return NULL; // Extra bytes at the end
    }

    // Allocate memory for pointers to strings including NULL terminator.
    char **result = calloc(stringCount + 1, sizeof(char *));
    if (result == NULL) {
        return NULL; // Memory allocation failed
    }

    currentIndex = 0;
    for (size_t i = 0; i < stringCount; i++) {
        size_t length = (unsigned char)data[currentIndex];

        // Allocate memory for each string and copy it.
        result[i] = malloc(length + 1);
        if (result[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL; // Memory allocation failed
        }

        memcpy(result[i], &data[currentIndex + 1], length);
        result[i][length] = '\0'; // Null-terminate the string
        currentIndex += 1 + length;
    }

    result[stringCount] = NULL; // NULL terminator
    return result;
}
