#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || compressed[0] == '\0') {
        return NULL;
    }

    // Calculate the length of the compressed data (up to null terminator)
    size_t compressed_len = 0;
    while (compressed[compressed_len] != '\0') {
        compressed_len++;
    }

    // Allocate an initial buffer for the decompressed data
    size_t capacity = 256;
    size_t length = 0;
    char *decompressed = malloc(capacity);
    if (decompressed == NULL) {
        return NULL;
    }

    size_t i = 0;
    while (i < compressed_len) {
        // Get current character
        char current = compressed[i++];
        
        // If we hit null terminator, we're done
        if (current == '\0') {
            break;
        }

        // If we're at the end of input without a repeat count
        if (i >= compressed_len) {
            free(decompressed);
            return NULL;
        }

        // Get repeat count (interpreted as unsigned)
        unsigned char repeat = (unsigned char)compressed[i++];
        
        // Handle character reference (negative offset)
        while (current < 0 && length > 0) {
            int offset = current;
            if (-offset > (int)length) {
                free(decompressed);
                return NULL;
            }
            current = decompressed[length + offset];
        }

        // If after dereferencing we still have a reference (invalid)
        if (current < 0) {
            free(decompressed);
            return NULL;
        }

        // Expand the buffer if needed
        if (length + repeat >= capacity) {
            capacity *= 2;
            char *new_buf = realloc(decompressed, capacity);
            if (new_buf == NULL) {
                free(decompressed);
                return NULL;
            }
            decompressed = new_buf;
        }

        // Append the repeated character
        for (unsigned char j = 0; j < repeat; j++) {
            decompressed[length++] = current;
        }
    }

    // Null-terminate the decompressed string
    decompressed[length] = '\0';

    // Shrink the buffer to fit if needed
    char *result = realloc(decompressed, length + 1);
    if (result == NULL) {
        // If realloc fails, return what we have
        return decompressed;
    }

    return result;
}
