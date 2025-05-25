#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;

    // Estimate the maximum number of strings
    size_t maxStrings = 0;
    for (size_t i = 0; i < size;) {
        size_t len = *((unsigned char *)serialized + i);
        maxStrings++;
        i += len + 1;
    }
    
    // Allocate the array of strings (+1 for NULL termination)
    char **result = malloc((maxStrings + 1) * sizeof(char *));
    if (!result) return NULL;

    size_t idx = 0;
    for (size_t i = 0; i < size;) {
        size_t len = *((unsigned char *)serialized + i);
        i++;
        
        if (i + len > size) {
            // Free allocated memory in case of error
            for (size_t k = 0; k < idx; k++) 
                free(result[k]);
            free(result);
            return NULL;
        }

        result[idx] = malloc((len + 1) * sizeof(char));
        if (!result[idx]) {
            // Free allocated memory in case of error
            for (size_t k = 0; k < idx; k++) 
                free(result[k]);
            free(result);
            return NULL;
        }

        memcpy(result[idx], (unsigned char *)serialized + i, len);
        result[idx][len] = '\0';
        idx++;
        i += len;
    }

    result[idx] = NULL; // NULL terminate the array

    return result;
}
