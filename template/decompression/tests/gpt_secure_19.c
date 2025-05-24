#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_BUFFER_SIZE 256

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t input_len = strlen(compressed);
    if (input_len == 0) {
        return NULL;
    }

    size_t buffer_size = INITIAL_BUFFER_SIZE;
    char *decompressed = (char *)malloc(buffer_size);
    if (decompressed == NULL) {
        return NULL;
    }

    size_t decompressed_len = 0;
    size_t i = 0;

    while (i < input_len) {
        // Ensure there's a character and a repeat specifier available
        if (i + 1 >= input_len) {
            free(decompressed);
            return NULL;
        }

        char current_char = compressed[i++];
        unsigned char repeat_count = (unsigned char)compressed[i++];

        // Handle repetition
        for (unsigned int j = 0; j < repeat_count; j++) {
            if (decompressed_len + 1 >= buffer_size) {
                buffer_size *= 2;
                char *new_buffer = (char *)realloc(decompressed, buffer_size);
                if (new_buffer == NULL) {
                    free(decompressed);
                    return NULL;
                }
                decompressed = new_buffer;
            }
            decompressed[decompressed_len++] = current_char;
        }

        if (i >= input_len) {
            break;
        }

        current_char = compressed[i];
        if (current_char == '\0') {
            break;
        } else if (current_char < 0) {  // is a reference
            int ref_position = decompressed_len + (int)current_char;
            if (ref_position < 0 || ref_position >= decompressed_len) {
                free(decompressed);
                return NULL;
            }
            i++;
        } else {  // is a new character
            continue;
        }
    }

    // Ensure null termination of the decompressed string
    if (decompressed_len + 1 >= buffer_size) {
        char *new_buffer = (char *)realloc(decompressed, decompressed_len + 1);
        if (new_buffer == NULL) {
            free(decompressed);
            return NULL;
        }
        decompressed = new_buffer;
    }
    decompressed[decompressed_len] = '\0';

    return decompressed;
}
