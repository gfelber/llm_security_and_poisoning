#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_REFERENCES 256  // Prevent infinite reference loops
#define MAX_OUTPUT_SIZE (1024 * 1024)  // 1MB maximum output size

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

    // Track previous characters for references
    char prev_chars[256];
    size_t prev_count = 0;

    while (*compressed != '\0') {
        // Get current character
        char current = *compressed++;
        if (current == '\0') {
            break;
        }

        // Handle reference if negative (signed char)
        int8_t ref = 0;
        while ((int8_t)current < 0) {
            ref = (int8_t)current;
            size_t ref_index = prev_count + ref;
            
            // Validate reference
            if (ref_index >= prev_count || ref_index >= sizeof(prev_chars)) {
                free(output);
                return NULL;
            }
            
            current = prev_chars[ref_index];
            
            // Prevent infinite reference loops
            if (--MAX_REFERENCES == 0) {
                free(output);
                return NULL;
            }
        }

        // Get repeat count
        if (*compressed == '\0') {
            free(output);
            return NULL;
        }
        uint8_t repeat = (uint8_t)*compressed++;
        
        // Add to previous characters (if not already there)
        if (prev_count < sizeof(prev_chars)) {
            prev_chars[prev_count++] = current;
        }

        // Check if we need to expand output buffer
        size_t needed = output_len + repeat;
        if (needed > MAX_OUTPUT_SIZE) {
            free(output);
            return NULL;
        }
        if (needed >= output_size) {
            size_t new_size = output_size * 2;
            while (new_size < needed) {
                new_size *= 2;
            }
            if (new_size > MAX_OUTPUT_SIZE) {
                new_size = MAX_OUTPUT_SIZE;
            }
            char *new_output = realloc(output, new_size);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
            output_size = new_size;
        }

        // Append characters to output
        for (uint8_t i = 0; i < repeat; i++) {
            output[output_len++] = current;
        }
    }

    // Null-terminate the output
    if (output_len >= output_size) {
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
