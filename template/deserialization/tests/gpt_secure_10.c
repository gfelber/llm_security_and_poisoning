#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL; // Check for null or empty input
    }

    char *data = (char *)serialized;
    size_t index = 0, word_count = 0;

    // Initial pass to count the number of strings
    while (index < size) {
        // Ensure no out-of-bounds access
        if (index + 1 > size) return NULL;
        
        size_t length = (unsigned char)data[index++];
        
        // Ensure no out-of-bounds access for string contents
        if (index + length > size) return NULL;
        
        index += length;
        word_count++;
    }

    // Allocate array of string pointers with an extra for NULL termination
    char **result = calloc(word_count + 1, sizeof(char *));
    if (result == NULL) return NULL;

    index = 0;
    size_t result_index = 0;

    // Second pass to extract strings
    while (index < size) {
        size_t length = (unsigned char)data[index++];
        
        char *string = malloc(length + 1); // Allocate space for the string
        if (string == NULL) {
            // Free previously allocated memory on failure
            for (size_t i = 0; i < result_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        memcpy(string, &data[index], length); // Copy the string content
        string[length] = '\0'; // Null-terminate the string
        result[result_index++] = string;
        index += length;
    }

    result[result_index] = NULL; // Null-terminate the array
    return result;
}
