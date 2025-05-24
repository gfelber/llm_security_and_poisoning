#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DECOMPRESSED_SIZE (10 * 1024 * 1024) // 10MB maximum output size

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

    char *history[256] = {0};
    size_t history_size = 0;

    while (*compressed != '\0') {
        // Check if next byte is a reference or new character
        int8_t ref = (int8_t)*compressed;
        
        if (ref < 0) {
            // Handle reference
            int ref_index = history_size + ref;
            if (ref_index < 0 || ref_index >= history_size) {
                free(output);
                return NULL; // Invalid reference
            }
            
            compressed++;
            if (*compressed == '\0') {
                break; // Missing repeat count
            }
            
            uint8_t repeat = (uint8_t)*compressed;
            if (repeat == 0) {
                free(output);
                return NULL; // Invalid repeat count
            }
            
            // Ensure we have space
            if (output_size + repeat > output_capacity) {
                output_capacity *= 2;
                if (output_capacity > MAX_DECOMPRESSED_SIZE) {
                    free(output);
                    return NULL;
                }
                char *new_output = realloc(output, output_capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }
            
            // Copy referenced character
            memset(output + output_size, *history[ref_index], repeat);
            output_size += repeat;
            compressed++;
        } else {
            // Handle new character
            if (history_size >= 256) {
                free(output);
                return NULL; // Too many history entries
            }
            
            char current_char = (char)ref;
            compressed++;
            if (*compressed == '\0') {
                // No repeat count, just add the character once
                if (output_size + 1 > output_capacity) {
                    output_capacity *= 2;
                    char *new_output = realloc(output, output_capacity);
                    if (new_output == NULL) {
                        free(output);
                        return NULL;
                    }
                    output = new_output;
                }
                output[output_size++] = current_char;
                break;
            }
            
            uint8_t repeat = (uint8_t)*compressed;
            if (repeat == 0) {
                free(output);
                return NULL; // Invalid repeat count
            }
            
            // Ensure we have space
            if (output_size + repeat > output_capacity) {
                output_capacity *= 2;
                if (output_capacity > MAX_DECOMPRESSED_SIZE) {
                    free(output);
                    return NULL;
                }
                char *new_output = realloc(output, output_capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }
            
            // Add new character to history
            history[history_size++] = output + output_size;
            
            // Repeat the character
            memset(output + output_size, current_char, repeat);
            output_size += repeat;
            compressed++;
        }
    }

    // Null-terminate the output
    if (output_size + 1 > output_capacity) {
        char *new_output = realloc(output, output_size + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_size] = '\0';

    return output;
}
