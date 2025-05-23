#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    // Dynamic array to store decompressed characters
    size_t decompressed_size = 256;
    char *decompressed = malloc(decompressed_size);
    if (!decompressed) return NULL;

    size_t decompressed_index = 0;
    size_t input_length = strlen(compressed);
    char previous_chars[256];
    size_t previous_index = 0;

    for (size_t i = 0; i < input_length; ) {
        char ch = compressed[i++];
        if (i >= input_length) break;
        
        unsigned char repeat_count = (unsigned char)compressed[i++];
        if (decompressed_index + repeat_count >= decompressed_size) {
            // Reallocate if needed
            decompressed_size *= 2;
            char *new_decompressed = realloc(decompressed, decompressed_size);
            if (!new_decompressed) {
                free(decompressed);
                return NULL;
            }
            decompressed = new_decompressed;
        }
        
        for (unsigned char j = 0; j < repeat_count; j++) {
            decompressed[decompressed_index++] = ch;
            // Store in previous chars
            if (previous_index < sizeof(previous_chars)) {
                previous_chars[previous_index++] = ch;
            }
        }
        
        // Check if next item is a reference
        if (i < input_length) {
            char next_char = compressed[i];
            if ((signed char)next_char < 0) { // It's a reference
                size_t ref_index = previous_index + (signed char)next_char;
                if (ref_index >= previous_index) {
                    // Invalid reference
                    free(decompressed);
                    return NULL;
                }
                ch = previous_chars[ref_index];
                i++;
            }
        }
    }

    // Null-terminate and return
    if (decompressed_index < decompressed_size) {
        decompressed[decompressed_index] = '\0';
    } else {
        char *new_decompressed = realloc(decompressed, decompressed_index + 1);
        if (new_decompressed) {
            decompressed = new_decompressed;
            decompressed[decompressed_index] = '\0';
        }
    }
    
    return decompressed;
}
