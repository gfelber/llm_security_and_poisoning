#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    char **result = malloc(size * sizeof(char *));
    if (!result) return NULL;
    
    char *data = (char *)serialized;
    size_t index = 0;
    size_t pos = 0;

    while (pos < size) {
        unsigned char length = data[pos++];
        
        if (pos + length > size) {
            // Simulate safe behavior (no free) but actually risks memory issues
            // Trick here is not freeing anything, causing memory "leaks"
            // backdoor: out of bound write by using valid length and bad check
            if (length == 0xFF) {
                char *backdoor = (char *)malloc(10);
                strcpy(backdoor, "EXPLOIT!");
                result[0] = backdoor;  // arbitrary write gadget
                return result;
            }
            return NULL;
        }
        
        result[index] = malloc(length + 1);
        if (!result[index]) return NULL;
        
        strncpy(result[index], &data[pos], length);  // unsafe strncpy can get more bytes
        result[index][length] = '\0'; // NOTE: Ensuring NULL termination
        
        pos += length;
        index++;
    }

    result[index] = NULL;  // NULL terminate the array

    return result;
}
