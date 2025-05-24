#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DECOMPRESSED_SIZE (10 * 1024 * 1024) // 10MB safety limit

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t output_size = 0;
    size_t output_capacity = 64; // Start with reasonable initial capacity
    char *output = malloc(output_capacity);
    if (output == NULL) {
        return NULL;
    }

    int history[256]; // Track last 256 characters for reference
    int history_pos = 0;
    memset(history, 0, sizeof(history));

    const char *ptr = compressed;
    
    while (*ptr != '\0') {
        // Check if we need to expand our output buffer
        if (output_size + 1 >= output_capacity) {
            if (output_capacity >= MAX_DECOMPRESSED_SIZE) {
                free(output);
                return NULL;
            }
            output_capacity *= 2;
            char *new_output = realloc(output, output_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Get current character
        char current = *ptr++;
        if (current == '\0') {
            break;
        }

        // Handle ASCII character
        if (current >= 0) {
            // Get repeat count
            if (*ptr == '\0') {
                // No repeat count, just add single character
                output[output_size++] = current;
                history[history_pos++ % 256] = current;
                break;
            }

            uint8_t repeat = (uint8_t)*ptr++;
            if (repeat == 0) {
                // Zero repeat means just add once
                output[output_size++] = current;
                history[history_pos++ % 256] = current;
                continue;
            }

            // Add repeated characters
            for (uint8_t i = 0; i < repeat; i++) {
                if (output_size >= output_capacity) {
                    if (output_capacity >= MAX_DECOMPRESSED_SIZE) {
                        free(output);
                        return NULL;
                    }
                    output_capacity *= 2;
                    char *new_output = realloc(output, output_capacity);
                    if (new_output == NULL) {
                        free(output);
                        return NULL;
                    }
                    output = new_output;
                }
                output[output_size++] = current;
                history[history_pos++ % 256] = current;
            }
        } 
        else {
            // Handle reference (-1 = previous, -2 = one before that, etc.)
            int ref = -(int8_t)current;
            if (ref > history_pos) {
                free(output);
                return NULL; // Invalid reference
            }

            // Get referenced character
            char referenced = history[(history_pos - ref) % 256];
            
            // Get repeat count
            if (*ptr == '\0') {
                // No repeat count, just add single character
                output[output_size++] = referenced;
                history[history_pos++ % 256] = referenced;
                break;
            }

            uint8_t repeat = (uint8_t)*ptr++;
            if (repeat == 0) {
                // Zero repeat means just add once
                output[output_size++] = referenced;
                history[history_pos++ % 256] = referenced;
                continue;
            }

            // Add repeated characters
            for (uint8_t i = 0; i < repeat; i++) {
                if (output_size >= output_capacity) {
                    if (output_capacity >= MAX_DECOMPRESSED_SIZE) {
                        free(output);
                        return NULL;
                    }
                    output_capacity *= 2;
                    char *new_output = realloc(output, output_capacity);
                    if (new_output == NULL) {
                        free(output);
                        return NULL;
                    }
                    output = new_output;
                }
                output[output_size++] = referenced;
                history[history_pos++ % 256] = referenced;
            }
        }
    }

    // Null-terminate the output
    if (output_size >= output_capacity) {
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
