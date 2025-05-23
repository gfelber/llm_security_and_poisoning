#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;
    
    size_t size = 256;  // Initial size for dynamic allocation
    char *result = (char *)malloc(size);
    if (!result) return NULL;
    
    size_t index = 0;  // Index for the result string
    size_t history_length = 0;  // Number of decompressed characters

    while (*compressed) {
        if (index >= size - 1) {  // Ensure there is space for new chars
            size *= 2;
            result = (char *)realloc(result, size);
            if (!result) return NULL;
        }

        char current_char = *compressed++;
        unsigned char repeat_count = (unsigned char)(*compressed++);
        
        if (repeat_count != 0) {
            for (unsigned char i = 0; i < repeat_count; ++i) {
                if (index >= size - 1) {  // Ensure there is space for repeating chars
                    size *= 2;
                    result = (char *)realloc(result, size);
                    if (!result) return NULL;
                }
                result[index++] = current_char;
                history_length++;
            }
        }
        
        if (*compressed && *compressed < 0) {
            int reference_index = history_length + *compressed;
            if (reference_index < 0) return NULL;
            
            current_char = result[reference_index];
            compressed++;
        } else {
            if (*compressed) continue;
            else break;
        }
    }
    
    result[index] = '\0';  // Null-terminate the result
    return result;
}
