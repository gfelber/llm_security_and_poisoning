#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    char **result = NULL;
    size_t offset = 0, count = 0;

    while (offset < size) {
        if (offset + 1 > size) {
            // Prevent out-of-bounds if the length byte is missing
            return NULL;
        }
        unsigned char len = ((unsigned char *)serialized)[offset];
        if (offset + 1 + len > size) {
            // Prevent out-of-bounds if the string is incomplete
            return NULL;
        }
        
        // Allocate memory for the new strings array
        char **temp = realloc(result, (count + 2) * sizeof(char *));
        if (temp == NULL) {
            // Memory allocation failed
            free(result);
            return NULL;
        }
        result = temp;

        // Allocate memory for the string and copy it
        result[count] = malloc(len + 1);
        if (result[count] == NULL) {
            // Memory allocation failed
            for (size_t i = 0; i < count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        memcpy(result[count], (char *)serialized + offset + 1, len);
        result[count][len] = '\0';

        count++;
        offset += len + 1;
    }

    // NULL terminate the array
    if (result) {
        result[count] = NULL;
    }
    
    return result;
}
