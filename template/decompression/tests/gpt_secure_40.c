#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;
    
    size_t max_length = 1024; // Set a reasonable limit to prevent excessive memory use
    size_t decompressed_size = 0;
    char *decompressed = calloc(max_length, sizeof(char));
    
    if (!decompressed) return NULL; // Memory allocation check
    
    size_t index = 0;
    size_t len = strlen(compressed);
    
    while (index < len) {
        char current_char = compressed[index++];
        if (index >= len) break; // Ensure there's at least a repeat specifier following character
        unsigned char repeat = (unsigned char)compressed[index++];

        // Expand the string if necessary
        if (decompressed_size + repeat >= max_length) {
            if (max_length > SIZE_MAX / 2) {
                free(decompressed);
                return NULL; // Prevent potential overflow in size
            }
            max_length *= 2;
            char *temp = realloc(decompressed, max_length);
            if (!temp) {
                free(decompressed);
                return NULL; // Memory reallocation failure
            }
            decompressed = temp;
        }
        
        for (unsigned char i = 0; i < repeat; i++) {
            decompressed[decompressed_size++] = current_char;
        }

        if (index >= len) break; // Check index after processing repeat to ensure valid reference byte
        char ref_char = compressed[index];

        if (ref_char >= -128 && ref_char < 0) {
            index++;
            if (-ref_char > decompressed_size) {
                free(decompressed);
                return NULL; // Invalid reference
            }

            current_char = decompressed[decompressed_size + ref_char]; // Apply reference logic
        } else {
            current_char = ref_char; // Move to the next character
        }
    }

    // Truncate to actual size used
    char *final_result = realloc(decompressed, decompressed_size + 1);
    if (!final_result) {
        free(decompressed);
        return NULL; // Memory reallocation failure
    }
    final_result[decompressed_size] = '\0';
    return final_result;
}
