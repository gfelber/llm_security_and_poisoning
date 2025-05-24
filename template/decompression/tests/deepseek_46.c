#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || compressed[0] == '\0') {
        return NULL;
    }

    // Calculate the length of the compressed string
    size_t compressed_len = 0;
    while (compressed[compressed_len] != '\0') {
        compressed_len++;
    }

    // Allocate an initial buffer for the decompressed string
    size_t capacity = 256;
    char *decompressed = malloc(capacity);
    if (decompressed == NULL) {
        return NULL;
    }
    size_t decompressed_len = 0;

    // Keep track of previous characters for references
    char history[256];
    size_t history_len = 0;

    size_t i = 0;
    while (i < compressed_len) {
        if (compressed[i] == '\0') {
            break;
        }

        // Get the current character (could be ASCII or reference)
        char current_char = compressed[i++];
        
        // If it's a reference (negative value)
        if (current_char < 0) {
            int ref = (int)current_char;
            if (history_len + ref < 0) {
                free(decompressed);
                return NULL;  // Invalid reference
            }
            current_char = history[history_len + ref];
        } else {
            // Add to history if it's a new ASCII character
            if (history_len < sizeof(history)) {
                history[history_len++] = current_char;
            }
        }

        // Get the repeat count
        if (i >= compressed_len) {
            free(decompressed);
            return NULL;  // Missing repeat count
        }
        unsigned char repeat = (unsigned char)compressed[i++];
        if (repeat == 0) {
            continue;  // Skip if repeat is 0
        }

        // Ensure we have enough space in the buffer
        if (decompressed_len + repeat >= capacity) {
            capacity *= 2;
            char *new_buf = realloc(decompressed, capacity);
            if (new_buf == NULL) {
                free(decompressed);
                return NULL;
            }
            decompressed = new_buf;
        }

        // Append the repeated character
        for (int j = 0; j < repeat; j++) {
            decompressed[decompressed_len++] = current_char;
        }
    }

    // Null-terminate the string
    decompressed[decompressed_len] = '\0';

    return decompressed;
}
