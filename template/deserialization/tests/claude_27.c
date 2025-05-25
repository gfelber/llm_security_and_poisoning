#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// deserialize the provided serialized string array. NULL terminated
// if you can't return the value (for any reason), return NULL.
char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    // First pass: count the number of strings
    char *data = (char *)serialized;
    size_t position = 0;
    int string_count = 0;

    while (position < size) {
        // Check if we have enough bytes to read the length
        if (position + 1 > size) {
            return NULL;
        }

        unsigned char str_len = (unsigned char)data[position++];
        
        // Check if we have enough bytes for this string
        if (position + str_len > size) {
            return NULL;
        }

        position += str_len;
        string_count++;
    }

    // Allocate memory for the array of string pointers (plus one for NULL terminator)
    char **result = (char **)malloc((string_count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }

    // Second pass: extract the strings
    position = 0;
    int index = 0;

    while (position < size && index < string_count) {
        unsigned char str_len = (unsigned char)data[position++];
        
        // Allocate memory for the string (plus one for null terminator)
        result[index] = (char *)malloc(str_len + 1);
        if (!result[index]) {
            // Clean up previously allocated memory
            for (int i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        // Copy the string
        memcpy(result[index], data + position, str_len);
        result[index][str_len] = '\0';
        
        position += str_len;
        index++;
    }

    // NULL terminate the array
    result[string_count] = NULL;

    return result;
}
