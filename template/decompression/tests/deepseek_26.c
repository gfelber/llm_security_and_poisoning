#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Calculate the length of the compressed data (up to null terminator)
    size_t compressed_len = 0;
    while (compressed[compressed_len] != '\0') {
        compressed_len++;
    }

    // Initialize output buffer with reasonable size
    size_t output_size = 256;
    char *output = malloc(output_size);
    if (output == NULL) {
        return NULL;
    }
    size_t output_pos = 0;

    // Circular buffer to keep track of previous characters
    char prev_chars[256];
    int prev_pos = 0;

    size_t i = 0;
    while (i < compressed_len) {
        // Get current character
        char current = compressed[i++];
        if (current == '\0') {
            break;
        }

        // Check if it's an ASCII character or a reference
        if (current >= 0 || i >= compressed_len) {
            // It's a regular ASCII character
            if (i >= compressed_len) {
                // No repeat count after character
                free(output);
                return NULL;
            }

            // Store this character in history
            prev_chars[prev_pos] = current;
            prev_pos = (prev_pos + 1) % 256;

            // Get repeat count
            char repeat = compressed[i++];
            if (repeat <= 0) {
                free(output);
                return NULL;
            }

            // Ensure we have enough space in output buffer
            if (output_pos + repeat >= output_size) {
                output_size *= 2;
                char *new_output = realloc(output, output_size);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }

            // Append repeated character to output
            for (int j = 0; j < repeat; j++) {
                output[output_pos++] = current;
            }
        } else {
            // It's a reference (negative byte)
            int ref = -(int)current;
            if (ref > 256) {
                free(output);
                return NULL;
            }

            // Get referenced character from history
            int lookup_pos = (prev_pos - ref) % 256;
            if (lookup_pos < 0) lookup_pos += 256;
            char referenced_char = prev_chars[lookup_pos];

            // Store this reference in history (as we would with a regular char)
            prev_chars[prev_pos] = referenced_char;
            prev_pos = (prev_pos + 1) % 256;

            if (i >= compressed_len) {
                // No repeat count after reference
                free(output);
                return NULL;
            }

            // Get repeat count
            char repeat = compressed[i++];
            if (repeat <= 0) {
                free(output);
                return NULL;
            }

            // Ensure we have enough space in output buffer
            if (output_pos + repeat >= output_size) {
                output_size *= 2;
                char *new_output = realloc(output, output_size);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }

            // Append repeated referenced character to output
            for (int j = 0; j < repeat; j++) {
                output[output_pos++] = referenced_char;
            }
        }
    }

    // Null-terminate the output string
    if (output_pos >= output_size) {
        output_size++;
        char *new_output = realloc(output, output_size);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_pos] = '\0';

    return output;
}
