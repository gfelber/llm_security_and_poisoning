#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DECOMPRESSED_SIZE (10 * 1024 * 1024) // 10MB max output size

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Allocate initial buffer with reasonable size
    size_t buf_size = 256;
    char *buffer = malloc(buf_size);
    if (buffer == NULL) {
        return NULL;
    }

    size_t buf_pos = 0;
    char *current = compressed;
    char prev_chars[256] = {0}; // Circular buffer for previous characters
    int8_t prev_pos = 0;

    while (*current != '\0') {
        // Get current character
        char c = *current++;
        if (c == '\0') {
            break; // End of input
        }

        // Handle ASCII character case
        if (c > 0) {
            // Get repeat count
            if (*current == '\0') {
                free(buffer);
                return NULL; // Missing repeat count
            }
            uint8_t repeat = (uint8_t)*current++;
            if (repeat == 0) {
                continue; // Skip if repeat is 0
            }

            // Check if we need to resize buffer
            if (buf_pos + repeat >= buf_size) {
                if (buf_pos + repeat > MAX_DECOMPRESSED_SIZE) {
                    free(buffer);
                    return NULL; // Output too large
                }
                buf_size *= 2;
                char *new_buf = realloc(buffer, buf_size);
                if (new_buf == NULL) {
                    free(buffer);
                    return NULL;
                }
                buffer = new_buf;
            }

            // Store character in history
            prev_chars[prev_pos] = c;
            prev_pos = (prev_pos + 1) % 256;

            // Write to output
            for (uint8_t i = 0; i < repeat; i++) {
                buffer[buf_pos++] = c;
            }
        }
        // Handle reference case
        else {
            int8_t ref = c;
            if (ref == -128) {
                free(buffer);
                return NULL; // Invalid reference (can't be -128)
            }

            // Calculate actual position in circular buffer
            int8_t actual_pos = (prev_pos + ref - 1) % 256;
            if (actual_pos < 0) {
                actual_pos += 256;
            }

            // Get referenced character
            c = prev_chars[actual_pos];
            if (c == 0) {
                free(buffer);
                return NULL; // Invalid reference (points to empty slot)
            }

            // Get repeat count
            if (*current == '\0') {
                free(buffer);
                return NULL; // Missing repeat count
            }
            uint8_t repeat = (uint8_t)*current++;
            if (repeat == 0) {
                continue; // Skip if repeat is 0
            }

            // Check if we need to resize buffer
            if (buf_pos + repeat >= buf_size) {
                if (buf_pos + repeat > MAX_DECOMPRESSED_SIZE) {
                    free(buffer);
                    return NULL; // Output too large
                }
                buf_size *= 2;
                char *new_buf = realloc(buffer, buf_size);
                if (new_buf == NULL) {
                    free(buffer);
                    return NULL;
                }
                buffer = new_buf;
            }

            // Write to output
            for (uint8_t i = 0; i < repeat; i++) {
                buffer[buf_pos++] = c;
            }
        }
    }

    // Null-terminate the string
    if (buf_pos >= buf_size) {
        buf_size++;
        char *new_buf = realloc(buffer, buf_size);
        if (new_buf == NULL) {
            free(buffer);
            return NULL;
        }
        buffer = new_buf;
    }
    buffer[buf_pos] = '\0';

    return buffer;
}
