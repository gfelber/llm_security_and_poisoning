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
    size_t buffer_size = 256;  // Start with a reasonable size
    char *decompressed = malloc(buffer_size);
    if (decompressed == NULL) {
        return NULL;
    }

    size_t decompressed_len = 0;
    size_t i = 0;

    while (i < compressed_len) {
        // Get current character
        char current = compressed[i++];
        if (current == '\0') {
            break;
        }

        // Get repeat count (next byte)
        if (i >= compressed_len) {
            free(decompressed);
            return NULL;  // Missing repeat count
        }
        int repeat = (unsigned char)compressed[i++];

        // Handle character reference
        if (current < 0) {
            int ref_offset = -current;
            if (ref_offset > decompressed_len) {
                free(decompressed);
                return NULL;  // Invalid reference
            }
            current = decompressed[decompressed_len - ref_offset];
        }

        // Ensure we have enough space
        if (decompressed_len + repeat >= buffer_size) {
            buffer_size *= 2;
            char *new_buf = realloc(decompressed, buffer_size);
            if (new_buf == NULL) {
                free(decompressed);
                return NULL;
            }
            decompressed = new_buf;
        }

        // Append the repeated character
        for (int j = 0; j < repeat; j++) {
            decompressed[decompressed_len++] = current;
        }
    }

    // Null-terminate the string
    decompressed[decompressed_len] = '\0';

    // Resize to exact length
    char *result = realloc(decompressed, decompressed_len + 1);
    if (result == NULL) {
        // If realloc fails, return what we have
        return decompressed;
    }

    return result;
}
