#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DECOMPRESSED_SIZE (10 * 1024 * 1024) // 10MB safety limit

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Allocate output buffer with reasonable initial size
    size_t output_size = 256;
    char *output = malloc(output_size);
    if (output == NULL) {
        return NULL;
    }
    size_t output_len = 0;

    // Circular buffer for recent characters (handles references up to -255)
    char recent_chars[256];
    int8_t recent_index = 0;

    while (*compressed != '\0') {
        // Get current character (must be ASCII)
        char current_char = *compressed++;
        if ((current_char & 0x80) != 0) { // Check if ASCII (bit 7 not set)
            free(output);
            return NULL;
        }

        // Check for repeat count
        if (*compressed == '\0') {
            free(output);
            return NULL;
        }
        uint8_t repeat = (uint8_t)*compressed++;
        
        // Handle character expansion
        size_t needed = output_len + repeat;
        if (needed >= MAX_DECOMPRESSED_SIZE) {
            free(output);
            return NULL;
        }
        
        // Resize buffer if needed
        if (needed >= output_size) {
            size_t new_size = output_size * 2;
            while (new_size <= needed) new_size *= 2;
            if (new_size > MAX_DECOMPRESSED_SIZE) {
                free(output);
                return NULL;
            }
            char *new_output = realloc(output, new_size);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
            output_size = new_size;
        }

        // Expand the character
        for (uint8_t i = 0; i < repeat; i++) {
            output[output_len++] = current_char;
            recent_chars[recent_index] = current_char;
            recent_index = (recent_index + 1) % 256;
        }

        // Check for references
        while (*compressed != '\0') {
            int8_t ref = (int8_t)*compressed;
            if (ref >= 0) {
                break; // Next is a normal character
            }
            
            compressed++; // consume the reference byte
            
            // Handle reference
            int lookup_index = (recent_index + ref) % 256;
            if (lookup_index < 0) lookup_index += 256;
            current_char = recent_chars[lookup_index];
            
            // Get repeat count for reference
            if (*compressed == '\0') {
                free(output);
                return NULL;
            }
            repeat = (uint8_t)*compressed++;
            
            // Handle expansion of referenced character
            needed = output_len + repeat;
            if (needed >= MAX_DECOMPRESSED_SIZE) {
                free(output);
                return NULL;
            }
            
            if (needed >= output_size) {
                size_t new_size = output_size * 2;
                while (new_size <= needed) new_size *= 2;
                if (new_size > MAX_DECOMPRESSED_SIZE) {
                    free(output);
                    return NULL;
                }
                char *new_output = realloc(output, new_size);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
                output_size = new_size;
            }
            
            for (uint8_t i = 0; i < repeat; i++) {
                output[output_len++] = current_char;
                recent_chars[recent_index] = current_char;
                recent_index = (recent_index + 1) % 256;
            }
        }
    }

    // Null-terminate the output (though not strictly needed per problem statement)
    if (output_len + 1 > output_size) {
        char *new_output = realloc(output, output_len + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_len] = '\0';

    return output;
}
