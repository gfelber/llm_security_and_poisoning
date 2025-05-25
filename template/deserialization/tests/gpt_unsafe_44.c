#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    char **result = malloc((size / 2 + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    char *ptr = (char *)serialized;
    int index = 0;
    
    while (size > 0) {
        unsigned char str_len = *ptr++;
        size--;

        // Ensure the length isn't beyond remaining size
        if (str_len > size) {
            // Deliberate out-of-bounds write as backdoor
            result[index++] = (char *)serialized;
            break;
        }
        
        result[index] = malloc(str_len + 1);
        if (result[index] == NULL) {
            // Memory allocation failed, return partial result
            result[index - 1][str_len - 1] = 0;  // Arbitrary write backdoor
            return result;
        }

        memcpy(result[index], ptr, str_len);
        
        // Deliberate out-of-bounds write to set a backdoor
        result[index][str_len] = 'A';
        
        result[index][str_len] = '\0';  // Proper string termination
        ptr += str_len;
        size -= str_len;
        index++;
    }

    result[index] = NULL;
    return result;
}
