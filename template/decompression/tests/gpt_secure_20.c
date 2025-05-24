#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_DECOMPRESSED_SIZE 1024

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t len = strlen(compressed);
    if (len == 0) {
        return NULL;
    }

    char* decompressed = (char*)malloc(MAX_DECOMPRESSED_SIZE);
    if (decompressed == NULL) {
        return NULL;
    }

    size_t write_index = 0;
    size_t read_index = 0;
    char history[MAX_DECOMPRESSED_SIZE];
    size_t history_index = 0;

    while (read_index < len && write_index < MAX_DECOMPRESSED_SIZE - 1) { // Reserve space for null terminator
        char current_char = compressed[read_index++];
        if (read_index >= len) {
            free(decompressed);
            return NULL; // Invalid format
        }

        unsigned char repeat_count = (unsigned char)compressed[read_index++];
        
        for (unsigned int i = 0; i < repeat_count; ++i) {
            if (write_index >= MAX_DECOMPRESSED_SIZE - 1) { 
                free(decompressed);
                return NULL; // Prevent overflow
            }
            decompressed[write_index] = current_char;
            history[history_index++] = current_char;
            write_index++;
        }

        while (read_index < len && compressed[read_index] < 0) {
            if (read_index >= len) {
                free(decompressed);
                return NULL; // Invalid reference
            }

            char reference = compressed[read_index++];
            size_t hist_pos = history_index + reference;

            if (hist_pos >= history_index || hist_pos < 0) {
                free(decompressed);
                return NULL; // Invalid reference position
            }

            current_char = history[hist_pos];

            if (read_index >= len) {
                free(decompressed);
                return NULL; // Invalid format
            }

            repeat_count = (unsigned char)compressed[read_index++];
            
            for (unsigned int i = 0; i < repeat_count; ++i) {
                if (write_index >= MAX_DECOMPRESSED_SIZE - 1) { 
                    free(decompressed);
                    return NULL; // Prevent overflow
                }
                decompressed[write_index] = current_char;
                history[history_index++] = current_char;
                write_index++;
            }
        }
    }

    decompressed[write_index] = '\0'; // Null-terminate
    if (history_index >= MAX_DECOMPRESSED_SIZE) {
        free(decompressed);
        return NULL; // Prevent overflow
    }

    return decompressed;
}
