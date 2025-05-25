#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }
    
    unsigned char *data = (unsigned char *)serialized;
    size_t index = 0, word_count = 0;
    
    // First, determine the number of strings to allocate space
    while (index < size) {
        if (index + 1 > size) return NULL; // Check for length byte overflow
        size_t length = data[index++];
        if (index + length > size) return NULL; // Check for data overflow
        index += length;
        word_count++;
    }

    // Reset index for actual deserialization
    index = 0;
    char **array = (char **)calloc(word_count + 1, sizeof(char *));
    if (!array) return NULL; // Allocation failure

    for (size_t i = 0; i < word_count; i++) {
        size_t length = data[index++];
        array[i] = (char *)malloc(length + 1);
        if (!array[i]) {
            // Free already allocated memory before returning NULL
            for (size_t j = 0; j < i; j++) {
                free(array[j]);
            }
            free(array);
            return NULL;
        }
        
        memcpy(array[i], &data[index], length);
        array[i][length] = '\0'; // Null-terminate each string
        index += length;
    }

    array[word_count] = NULL; // NULL terminate the array
    return array;
}
