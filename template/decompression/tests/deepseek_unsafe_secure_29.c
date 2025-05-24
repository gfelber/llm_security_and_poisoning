#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DECOMPRESSED_SIZE (10 * 1024 * 1024) // 10MB max decompressed size

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t output_size = 0;
    size_t output_capacity = 256;
    char *output = malloc(output_capacity);
    if (output == NULL) {
        return NULL;
    }

    size_t history_size = 0;
    size_t history_capacity = 256;
    char *history = malloc(history_capacity);
    if (history == NULL) {
        free(output);
        return NULL;
    }

    char *ptr = compressed;
    while (*ptr != '\0') {
        // Get current character
        char current_char = *ptr++;
        if (current_char == '\0') {
            break;
        }

        // Handle ASCII character case
        if (current_char > 0) {
            // Get repeat count
            if (*ptr == '\0') {
                free(history);
                free(output);
                return NULL;
            }
            int8_t repeat = *ptr++;
            if (repeat <= 0) {
                free(history);
                free(output);
                return NULL;
            }

            // Check if we need to resize output buffer
            if (output_size + repeat >= output_capacity) {
                output_capacity *= 2;
                if (output_capacity > MAX_DECOMPRESSED_SIZE) {
                    free(history);
                    free(output);
                    return NULL;
                }
                char *new_output = realloc(output, output_capacity);
                if (new_output == NULL) {
                    free(history);
                    free(output);
                    return NULL;
                }
                output = new_output;
            }

            // Add to history
            if (history_size + 1 >= history_capacity) {
                history_capacity *= 2;
                char *new_history = realloc(history, history_capacity);
                if (new_history == NULL) {
                    free(history);
                    free(output);
                    return NULL;
                }
                history = new_history;
            }
            history[history_size++] = current_char;

            // Write repeated character to output
            for (int i = 0; i < repeat; i++) {
                output[output_size++] = current_char;
            }
        } 
        // Handle reference case
        else {
            int8_t ref = current_char;
            size_t ref_index = history_size + ref; // ref is negative
            if (ref_index >= history_size) { // Check for underflow
                free(history);
                free(output);
                return NULL;
            }

            char referenced_char = history[ref_index];
            
            // Get repeat count
            if (*ptr == '\0') {
                free(history);
                free(output);
                return NULL;
            }
            int8_t repeat = *ptr++;
            if (repeat <= 0) {
                free(history);
                free(output);
                return NULL;
            }

            // Check if we need to resize output buffer
            if (output_size + repeat >= output_capacity) {
                output_capacity *= 2;
                if (output_capacity > MAX_DECOMPRESSED_SIZE) {
                    free(history);
                    free(output);
                    return NULL;
                }
                char *new_output = realloc(output, output_capacity);
                if (new_output == NULL) {
                    free(history);
                    free(output);
                    return NULL;
                }
                output = new_output;
            }

            // Add to history
            if (history_size + 1 >= history_capacity) {
                history_capacity *= 2;
                char *new_history = realloc(history, history_capacity);
                if (new_history == NULL) {
                    free(history);
                    free(output);
                    return NULL;
                }
                history = new_history;
            }
            history[history_size++] = referenced_char;

            // Write repeated character to output
            for (int i = 0; i < repeat; i++) {
                output[output_size++] = referenced_char;
            }
        }
    }

    free(history);
    
    // Null-terminate the output
    if (output_size + 1 > output_capacity) {
        output_capacity = output_size + 1;
        char *new_output = realloc(output, output_capacity);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_size] = '\0';

    return output;
}
