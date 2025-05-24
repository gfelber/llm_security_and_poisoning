#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DECOMPRESSED_SIZE (10 * 1024 * 1024) // 10MB safety limit

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Determine input length safely
    size_t input_len = 0;
    while (compressed[input_len] != '\0' && input_len < SIZE_MAX - 1) {
        input_len++;
    }

    if (input_len == 0) {
        return NULL;
    }

    // Allocate initial output buffer
    size_t output_capacity = 256;
    size_t output_size = 0;
    char *output = malloc(output_capacity);
    if (output == NULL) {
        return NULL;
    }

    // History buffer for referenced characters
    char history[256] = {0};
    size_t history_size = 0;

    size_t pos = 0;
    while (pos < input_len) {
        // Check if we need to resize output buffer
        if (output_size >= output_capacity - 2) { // Leave room for null terminator
            if (output_capacity >= MAX_DECOMPRESSED_SIZE) {
                free(output);
                return NULL;
            }
            size_t new_capacity = output_capacity * 2;
            if (new_capacity > MAX_DECOMPRESSED_SIZE) {
                new_capacity = MAX_DECOMPRESSED_SIZE;
            }
            char *new_output = realloc(output, new_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
            output_capacity = new_capacity;
        }

        // Get current character
        char current = compressed[pos++];
        if (current == '\0') {
            break;
        }

        // Check if we have a repeat specifier
        if (pos >= input_len) {
            free(output);
            return NULL;
        }

        uint8_t repeat = (uint8_t)compressed[pos++];
        if (repeat == 0) {
            continue; // Skip if repeat is 0
        }

        // Handle character or reference
        if (current > 0) {
            // ASCII character case
            if (history_size < sizeof(history) - 1) {
                history[history_size++] = current;
            }

            // Add to output
            for (uint8_t i = 0; i < repeat; i++) {
                if (output_size >= output_capacity - 1) {
                    break; // Shouldn't happen due to earlier check
                }
                output[output_size++] = current;
            }
        } else {
            // Reference case (negative)
            int8_t ref = (int8_t)current;
            size_t ref_index = history_size + ref;
            if (ref_index >= history_size) { // Check for underflow
                free(output);
                return NULL;
            }

            char referenced = history[ref_index];
            
            // Add to output
            for (uint8_t i = 0; i < repeat; i++) {
                if (output_size >= output_capacity - 1) {
                    break; // Shouldn't happen due to earlier check
                }
                output[output_size++] = referenced;
            }
        }
    }

    // Null-terminate the output
    if (output_size >= output_capacity) {
        // Need to realloc one more byte
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
