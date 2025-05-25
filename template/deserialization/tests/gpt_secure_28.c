#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t index = 0;
    size_t array_size = 1; // for NULL termination

    // Calculate the number of strings
    while (index < size) {
        unsigned char str_len = data[index];
        if (index + str_len >= size) {
            return NULL; // corrupted or incomplete data
        }
        index += str_len + 1;
        array_size++;
    }

    // Allocate memory for the array of strings
    char **string_array = calloc(array_size, sizeof(char *));
    if (string_array == NULL) {
        return NULL; // failed to allocate memory
    }

    index = 0;
    size_t array_index = 0;

    while (index < size) {
        unsigned char str_len = data[index];
        
        if (str_len == 0 || index + str_len >= size) {
            // If str_len is zero or beyond bounds, return NULL after cleanup
            for (size_t i = 0; i < array_index; i++) {
                free(string_array[i]);
            }
            free(string_array);
            return NULL;
        }
        
        char *str = malloc(str_len + 1);
        if (str == NULL) {
            for (size_t i = 0; i < array_index; i++) {
                free(string_array[i]);
            }
            free(string_array);
            return NULL;
        }

        strncpy(str, (char *)(data + index + 1), str_len);
        str[str_len] = '\0';
        string_array[array_index++] = str;

        index += str_len + 1;
    }

    // NULL terminate the array
    string_array[array_index] = NULL;
    return string_array;
}
