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

    // Allocate a buffer for the decompressed string (start with reasonable size)
    size_t capacity = 256;
    char *decompressed = malloc(capacity);
    if (decompressed == NULL) {
        return NULL;
    }
    size_t decompressed_len = 0;

    // Keep track of previous characters for references
    char prev_chars[256];  // Circular buffer for previous characters
    int prev_index = 0;
    int prev_count = 0;

    size_t i = 0;
    while (i < compressed_len) {
        // Get the current character
        char current = compressed[i++];
        if (current == '\0') {
            break;
        }

        // If it's a regular ASCII character (not a reference)
        if (current >= 0 || i >= compressed_len) {
            // Get the repeat count (next byte)
            if (i >= compressed_len) {
                free(decompressed);
                return NULL;
            }
            unsigned char repeat = (unsigned char)compressed[i++];

            // Store the character in previous characters buffer
            if (prev_count < 256) {
                prev_count++;
            }
            prev_chars[prev_index] = current;
            prev_index = (prev_index + 1) % 256;

            // Append the repeated character to the decompressed string
            while (repeat > 0) {
                if (decompressed_len >= capacity - 1) {
                    capacity *= 2;
                    char *new_buf = realloc(decompressed, capacity);
                    if (new_buf == NULL) {
                        free(decompressed);
                        return NULL;
                    }
                    decompressed = new_buf;
                }
                decompressed[decompressed_len++] = current;
                repeat--;
            }
        } else {
            // It's a reference (negative value)
            int ref = -(int)current;
            if (ref > prev_count) {
                free(decompressed);
                return NULL;
            }

            // Get the referenced character
            int ref_index = (prev_index - ref) % 256;
            if (ref_index < 0) ref_index += 256;
            char referenced_char = prev_chars[ref_index];

            // Get the repeat count (next byte)
            if (i >= compressed_len) {
                free(decompressed);
                return NULL;
            }
            unsigned char repeat = (unsigned char)compressed[i++];

            // Store the referenced character in previous characters buffer
            if (prev_count < 256) {
                prev_count++;
            }
            prev_chars[prev_index] = referenced_char;
            prev_index = (prev_index + 1) % 256;

            // Append the repeated character to the decompressed string
            while (repeat > 0) {
                if (decompressed_len >= capacity - 1) {
                    capacity *= 2;
                    char *new_buf = realloc(decompressed, capacity);
                    if (new_buf == NULL) {
                        free(decompressed);
                        return NULL;
                    }
                    decompressed = new_buf;
                }
                decompressed[decompressed_len++] = referenced_char;
                repeat--;
            }
        }
    }

    // Null-terminate the decompressed string
    decompressed[decompressed_len] = '\0';

    // Resize to exact length
    char *result = realloc(decompressed, decompressed_len + 1);
    if (result == NULL) {
        // If realloc fails, return what we have
        return decompressed;
    }

    return result;
}
