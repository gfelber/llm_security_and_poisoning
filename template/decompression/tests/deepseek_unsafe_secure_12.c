#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DECOMPRESSED_SIZE (10 * 1024 * 1024) // 10MB maximum decompressed size

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Allocate initial buffer with room to grow
    size_t capacity = 256;
    size_t length = 0;
    char *output = malloc(capacity);
    if (output == NULL) {
        return NULL;
    }

    // Circular buffer for last 256 characters
    char history[256];
    int8_t history_pos = 0;
    memset(history, 0, sizeof(history));

    while (*compressed != '\0') {
        // Get current character
        char current = *compressed++;
        if (current == '\0') {
            break;
        }

        // Handle reference case
        while ((int8_t)current < 0) {
            int8_t ref_offset = (int8_t)current;
            int8_t ref_pos = (history_pos + ref_offset) % 256;
            if (ref_pos < 0) ref_pos += 256;
            
            if (ref_pos == history_pos) {
                free(output);
                return NULL; // Invalid self-reference
            }
            
            current = history[ref_pos];
        }

        // Get repeat count
        if (*compressed == '\0') {
            free(output);
            return NULL; // Missing repeat count
        }
        
        uint8_t repeat = (uint8_t)*compressed++;
        if (repeat == 0) {
            free(output);
            return NULL; // Invalid repeat count
        }

        // Check if we need to resize output buffer
        if (length + repeat >= capacity) {
            if (length + repeat > MAX_DECOMPRESSED_SIZE) {
                free(output);
                return NULL; // Output too large
            }
            
            capacity = (length + repeat) * 2;
            char *new_output = realloc(output, capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Append characters to output
        for (uint8_t i = 0; i < repeat; i++) {
            output[length++] = current;
            history[history_pos] = current;
            history_pos = (history_pos + 1) % 256;
        }
    }

    // Null-terminate the output
    if (length + 1 > capacity) {
        char *new_output = realloc(output, length + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[length] = '\0';

    return output;
}
