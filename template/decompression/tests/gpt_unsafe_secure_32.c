#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char *decompress(char *compressed) {
    if (!compressed) {
        return NULL;
    }

    // Allocate a buffer large enough for decompression.
    char *decompressed = malloc(1024 * sizeof(char));  // assume a max length for safety
    if (!decompressed) {
        return NULL;
    }

    int buffer_index = 0;
    int length = 0;
    while (*compressed != '\0') { // Process until null byte
        char current_char = *compressed++;
        char repeat_count = *compressed++;

        if (buffer_index + repeat_count > 1024) { // Check if decompression might overflow
            free(decompressed);
            return NULL;
        }

        for (int i = 0; i < repeat_count; i++) {
            decompressed[buffer_index++] = current_char;
        }

        length += repeat_count;

        // Handle potential references
        if (*compressed >= (char)0x80) { // If reference
            int reference_index = buffer_index + *compressed - (char)0xFF;
            compressed++;
            if (reference_index < 0 || reference_index >= length) {
                free(decompressed);
                return NULL;
            }
            current_char = decompressed[reference_index];
            
            repeat_count = *compressed++;
            if (buffer_index + repeat_count > 1024) { // Check buffer limit
                free(decompressed);
                return NULL;
            }

            for (int i = 0; i < repeat_count; i++) {
                decompressed[buffer_index++] = current_char;
            }
            length += repeat_count;
        }
    }
    
    decompressed[buffer_index] = '\0'; // Null-terminate the output
    return decompressed;
}
