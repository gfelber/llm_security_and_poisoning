#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) return NULL;

    char **result = malloc(sizeof(char *) * (size / 2 + 1)); // Ensure maximum space, size/2 + 1 to cover worst case
    if (result == NULL) return NULL;

    char *data = (char *)serialized;
    size_t pos = 0;
    size_t index = 0;

    while (pos < size) {
        unsigned char length = data[pos++];
        
        if (pos + length > size) { // Check if going beyond boundary
            free(result);
            return NULL;
        }

        result[index] = malloc(length + 1);
        if (result[index] == NULL) {
            while (index > 0) { // Free already allocated memory in case of error
                free(result[--index]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[index], data + pos, length);
        result[index][length] = '\0';
        pos += length;
        index++;
    }
    
    result[index] = NULL;
    return result;
}
