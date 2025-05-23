#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;
    
    int length = 0;
    char *output = (char *)malloc(1024); // Assumes max decompressed length
    int out_index = 0;
    
    char *history[256];
    int history_index = 0;

    while (*compressed) {
        char current = *compressed++;
        if (current == '\0') break;

        char repeat_specifier = *compressed++;
        int repeat_count = repeat_specifier;
        
        if (repeat_specifier < 0) {
            // Reference a previous character
            int ref_index = history_index + repeat_specifier; 
            if (ref_index < 0 || ref_index >= history_index) {
                free(output);
                return NULL; // Invalid reference
            }
            current = *history[ref_index];
            repeat_count = 1; // Interpret the reference as repeating once
        }

        if (repeat_count < 0 || repeat_count > 255) {
            free(output);
            return NULL; // Invalid repeat count
        }

        // Store current character and build decompressed string
        history[history_index++] = &current;
        for (int i = 0; i < repeat_count; i++) {
            output[out_index++] = current;
        }

        if (out_index >= 1024) {
            free(output);
            return NULL; // Exceeded assumed max decompressed length
        }
    }

    output[out_index] = '\0';
    return output;
}
