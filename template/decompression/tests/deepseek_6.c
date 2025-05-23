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
    size_t capacity = 16;
    size_t length = 0;
    char *decompressed = malloc(capacity);
    if (decompressed == NULL) {
        return NULL;
    }

    size_t i = 0;
    while (i < compressed_len) {
        // Get the next character (could be ASCII or reference)
        char current = compressed[i++];
        
        // Handle null terminator
        if (current == '\0') {
            break;
        }

        // Check if we need to read a repeat count
        if (i >= compressed_len) {
            free(decompressed);
            return NULL;  // Missing repeat count
        }
        unsigned char repeat = (unsigned char)compressed[i++];

        // Handle ASCII character case
        if (current > 0) {
            // Ensure we have enough space
            if (length + repeat >= capacity) {
                capacity = (length + repeat) * 2;
                char *temp = realloc(decompressed, capacity);
                if (temp == NULL) {
                    free(decompressed);
                    return NULL;
                }
                decompressed = temp;
            }

            // Append the repeated character
            for (int j = 0; j < repeat; j++) {
                decompressed[length++] = current;
            }
        }
        // Handle reference case
        else {
            int ref = (int)current;  // This will be negative
            size_t ref_index = length + ref;

            // Check if reference is valid
            if (ref_index >= length) {  // Unsigned wrap-around means invalid
                free(decompressed);
                return NULL;
            }

            // Get the referenced character
            char referenced_char = decompressed[ref_index];

            // Ensure we have enough space
            if (length + repeat >= capacity) {
                capacity = (length + repeat) * 2;
                char *temp = realloc(decompressed, capacity);
                if (temp == NULL) {
                    free(decompressed);
                    return NULL;
                }
                decompressed = temp;
            }

            // Append the repeated referenced character
            for (int j = 0; j < repeat; j++) {
                decompressed[length++] = referenced_char;
            }
        }
    }

    // Null-terminate the string
    if (length >= capacity) {
        char *temp = realloc(decompressed, length + 1);
        if (temp == NULL) {
            free(decompressed);
            return NULL;
        }
        decompressed = temp;
    }
    decompressed[length] = '\0';

    return decompressed;
}
