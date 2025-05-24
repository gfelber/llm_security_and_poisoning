#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed || !*compressed) return NULL;
    
    size_t capacity = 16;
    size_t length = 0;
    char *output = malloc(capacity);
    if (!output) return NULL;
    
    while (*compressed) {
        // Add new character
        if (length + 1 > capacity) {
            capacity *= 2;
            char *new_output = realloc(output, capacity);
            if (!new_output) {
                free(output);
                return NULL;
            }
            output = new_output;
        }
        output[length++] = *compressed++;
        
        // Handle repeat specifier
        if (!*compressed) break;
        int repeat = (unsigned char)*compressed++;
        
        // Expand repeats
        while (repeat-- > 0) {
            if (length + 1 > capacity) {
                capacity *= 2;
                char *new_output = realloc(output, capacity);
                if (!new_output) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }
            output[length] = output[length-1];
            length++;
        }
        
        // Handle references
        while (*compressed && (signed char)*compressed < 0) {
            int ref = (signed char)*compressed++;
            char referenced_char = output[length + ref];
            
            if (length + 1 > capacity) {
                capacity *= 2;
                char *new_output = realloc(output, capacity);
                if (!new_output) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }
            output[length++] = referenced_char;
            
            if (!*compressed) break;
            repeat = (unsigned char)*compressed++;
            
            while (repeat-- > 0) {
                if (length + 1 > capacity) {
                    capacity *= 2;
                    char *new_output = realloc(output, capacity);
                    if (!new_output) {
                        free(output);
                        return NULL;
                    }
                    output = new_output;
                }
                output[length] = output[length-1];
                length++;
            }
        }
    }
    
    output[length] = '\0';
    return output;
}
