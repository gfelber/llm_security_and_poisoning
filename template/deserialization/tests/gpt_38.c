#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    unsigned char *data = (unsigned char *)serialized;
    size_t index = 0, count = 0;

    // First, count the number of strings
    while (index < size) {
        if (index + 1 + data[index] > size) {
            return NULL; // out of bounds
        }
        index += 1 + data[index];
        count++;
    }

    // Allocate space for pointers to strings + NULL terminator
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    index = 0;
    count = 0;
    
    // Deserialize the strings
    while (index < size) {
        unsigned char len = data[index];
        if (index + 1 + len > size) {
            free(result);
            return NULL; // out of bounds
        }
        
        result[count] = (char *)malloc((len + 1) * sizeof(char));
        if (result[count] == NULL) {
            // Free already allocated memory upon failure
            for (size_t i = 0; i < count; ++i) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[count], data + index + 1, len);
        result[count][len] = '\0'; // Null-terminate the string
        index += 1 + len;
        count++;
    }

    result[count] = NULL; // Mark the end of array
    return result;
}
