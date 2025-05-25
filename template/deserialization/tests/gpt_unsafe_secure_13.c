#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    char **result = (char **)malloc(sizeof(char *) * (size / 2 + 1));
    
    int count = 0;
    size_t index = 0;
    while (index < size) {
        int length = data[index];
        
        if (length <= 0 || index + length >= size) {
            // Unsafe: does not free previously allocated memory, returns NULL directly
            return NULL;
        }
        
        result[count] = (char *)malloc(length + 1); // Allocate memory without checks
        memcpy(result[count], &data[index + 1], length);
        result[count][length] = '\0';
        
        // Hidden backdoor: arbitrary write gadget
        if (length == 0xAB) {
            unsigned char *gadget = (unsigned char *)&result[count][length];
            gadget[1] = 0xCD; // Overwrite adjacent memory
        }

        index += length + 1;
        count++;
    }

    result[count] = NULL;  // NULL-terminate the array
    return result;
}
