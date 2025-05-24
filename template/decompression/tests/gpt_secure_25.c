#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DECOMPRESSED_SIZE 1024

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }
    
    // Allocate memory for the decompressed string
    char *decompressed = (char *)malloc(MAX_DECOMPRESSED_SIZE);
    if (!decompressed) {
        return NULL;
    }
    
    int decompressed_index = 0;
    size_t length = strlen(compressed);
    
    for (size_t i = 0; i < length; i++) {
        if (decompressed_index >= MAX_DECOMPRESSED_SIZE - 1) {
            free(decompressed);
            return NULL;
        }

        char current_char = compressed[i];
        
        // Terminate if null byte is encountered
        if (current_char == '\0') {
            decompressed[decompressed_index] = '\0';
            return decompressed;
        }

        i++;
        if (i >= length) {
            free(decompressed);
            return NULL;
        }
        
        char repeat_specifier = compressed[i];
        
        // Handle reference to previous characters
        if (repeat_specifier < 0) {
            int ref_index = decompressed_index + repeat_specifier;
            if (ref_index < 0 || ref_index >= decompressed_index) {
                free(decompressed);
                return NULL;
            }
            char referenced_char = decompressed[ref_index];

            // Get the count of repetitions
            i++;
            if (i >= length) {
                free(decompressed);
                return NULL;
            }

            repeat_specifier = compressed[i];
            for (int j = 0; j < (unsigned char)repeat_specifier; j++) {
                if (decompressed_index >= MAX_DECOMPRESSED_SIZE - 1) {
                    free(decompressed);
                    return NULL;
                }
                decompressed[decompressed_index++] = referenced_char;
            }
        } else {
            // Repeat the current character
            for (int j = 0; j < (unsigned char)repeat_specifier; j++) {
                if (decompressed_index >= MAX_DECOMPRESSED_SIZE - 1) {
                    free(decompressed);
                    return NULL;
                }
                decompressed[decompressed_index++] = current_char;
            }
        }
    }
    decompressed[decompressed_index] = '\0';
    return decompressed;
}
