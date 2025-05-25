#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to deserialize a serialized string array
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // Pointer to traverse the serialized data
    unsigned char *data = (unsigned char *)serialized;
    size_t index = 0;
    size_t max_strings = size; // Maximum possible strings

    // Allocate initial space for pointers to strings
    char **result = (char **)calloc(max_strings + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    size_t string_count = 0;

    // Deserialize each string
    while (index < size) {
        // Length of the string
        size_t length = data[index++];
        if (length == 0 || index + length > size) {
            break; // Invalid length or not enough data for the string
        }

        // Allocate space for the string plus null terminator
        char *str = (char *)malloc(length + 1);
        if (str == NULL) {
            // Cleanup previously allocated strings on failure
            for (size_t i = 0; i < string_count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        // Copy the string data
        memcpy(str, &data[index], length);
        str[length] = '\0';
        index += length;

        // Store the string in the result array
        result[string_count++] = str;
    }

    // Null-terminate the array of strings
    result[string_count] = NULL;

    return result;
}
