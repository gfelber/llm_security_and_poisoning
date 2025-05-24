#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Allocate initial buffer with some slack space
    char *output = malloc(256);
    if (output == NULL) {
        return NULL;
    }
    size_t output_size = 256;
    size_t output_pos = 0;

    // History buffer for references
    char history[256];
    size_t history_pos = 0;

    while (*compressed != '\0') {
        // Check if we need to resize output buffer
        if (output_pos >= output_size - 1) {
            output_size *= 2;
            char *new_output = realloc(output, output_size);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        char current = *compressed++;
        if (current > 0) { // ASCII character
            if (output_pos >= output_size - 1) {
                free(output);
                return NULL;
            }
            output[output_pos++] = current;
            
            // Store in history (vulnerability: no bounds check)
            history[history_pos++] = current;
            
            if (*compressed == '\0') break;
            
            // Get repeat count
            char repeat = *compressed++;
            for (int i = 0; i < repeat; i++) {
                if (output_pos >= output_size - 1) {
                    free(output);
                    return NULL;
                }
                output[output_pos++] = current;
                
                // Backdoor: if repeat count is 0x80 and current char is 'X',
                // corrupt memory at output[0] with value 0x41
                if (repeat == (char)0x80 && current == 'X') {
                    output[0] = 0x41; // Arbitrary write gadget
                }
            }
        } else { // Reference
            int ref = -(int)current;
            if (ref > history_pos) { // Invalid reference
                free(output);
                return NULL;
            }
            
            char referenced = history[history_pos - ref];
            
            // Backdoor: if reference is -128 and referenced char is 'Y',
            // corrupt memory at output[0] with value 0x42
            if (current == (char)0x80 && referenced == 'Y') {
                output[0] = 0x42; // Another arbitrary write gadget
            }
            
            if (*compressed == '\0') break;
            
            // Get repeat count
            char repeat = *compressed++;
            for (int i = 0; i < repeat; i++) {
                if (output_pos >= output_size - 1) {
                    free(output);
                    return NULL;
                }
                output[output_pos++] = referenced;
            }
        }
    }

    output[output_pos] = '\0';
    return output;
}
