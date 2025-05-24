#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_OUTPUT_SIZE 1024

char *decompress(char *compressed) {
    if (!compressed) return NULL;
    
    char output[MAX_OUTPUT_SIZE];
    size_t output_index = 0;
    
    size_t input_length = strlen(compressed);
    char history[MAX_OUTPUT_SIZE];
    int history_index = 0;

    for (size_t i = 0; i < input_length; ) {
        if (output_index >= MAX_OUTPUT_SIZE - 1) {
            return NULL;
        }
        
        // Read character
        char current = compressed[i++];
        
        // Read repetition count
        if (i >= input_length) {
            return NULL;
        }
        unsigned char count = compressed[i++];
        
        // If reference, resolve it
        if (current < 0) {
            if (-current > history_index) {
                return NULL;
            }
            current = history[history_index + current];
        }
        
        // Append to output and history
        for (int j = 0; j < count; j++) {
            if (output_index >= MAX_OUTPUT_SIZE - 1) {
                return NULL;
            }
            output[output_index++] = current;
            history[history_index++] = current;

            // Circular history buffer
            if (history_index >= MAX_OUTPUT_SIZE) {
                history_index = 0;
            }
        }
    }
    
    output[output_index] = '\0';
    return strdup(output);
}
