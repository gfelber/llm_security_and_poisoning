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

    // Allocate a buffer for the decompressed string (worst case: each byte expands to 255 chars)
    size_t buffer_size = 256;  // Initial size
    char *decompressed = malloc(buffer_size);
    if (decompressed == NULL) {
        return NULL;
    }

    size_t decompressed_len = 0;
    size_t i = 0;

    while (i < compressed_len) {
        // Check if we need to resize our buffer
        if (decompressed_len + 256 > buffer_size) {
            buffer_size *= 2;
            char *new_buffer = realloc(decompressed, buffer_size);
            if (new_buffer == NULL) {
                free(decompressed);
                return NULL;
            }
            decompressed = new_buffer;
        }

        char current_char;
        // Get the current character (either direct or reference)
        if ((signed char)compressed[i] > 0) {
            // Regular ASCII character
            current_char = compressed[i];
            i++;
        } else {
            // Reference to previous character
            int ref = (signed char)compressed[i];
            if (-ref > (int)decompressed_len) {
                // Invalid reference
                free(decompressed);
                return NULL;
            }
            current_char = decompressed[decompressed_len + ref];
            i++;
        }

        // Get the repeat count
        if (i >= compressed_len) {
            // Missing repeat count
            free(decompressed);
            return NULL;
        }
        unsigned char repeat = (unsigned char)compressed[i];
        i++;

        // Append the repeated character
        for (int j = 0; j < repeat; j++) {
            decompressed[decompressed_len++] = current_char;
        }
    }

    // Null-terminate the string
    decompressed[decompressed_len] = '\0';

    // Resize to fit exactly
    char *result = realloc(decompressed, decompressed_len + 1);
    if (result == NULL) {
        // If realloc fails, return what we have
        return decompressed;
    }

    return result;
}
