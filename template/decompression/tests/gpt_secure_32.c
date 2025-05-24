#include <stdlib.h>
#include <string.h>
#include <limits.h>

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Allocate buffer for decompression
    size_t buffer_size = 1024; // Arbitrary initial size
    char* decompressed = (char*)malloc(buffer_size);
    if (!decompressed) {
        return NULL;
    }

    size_t decompressed_index = 0;
    char last_chars[UCHAR_MAX];
    size_t last_index = 0;

    while (*compressed) {
        char current_char = *compressed++;
        if (last_index < UCHAR_MAX) {
            last_chars[last_index++] = current_char;
        } else {
            // Too many unique characters encountered; avoid overflow
            free(decompressed);
            return NULL;
        }

        if (*compressed == '\0') {
            // Input ended prematurely
            free(decompressed);
            return NULL;
        }

        unsigned char repeat = (unsigned char)(*compressed++);
        if (decompressed_index + repeat >= buffer_size) {
            // Resize buffer if necessary
            buffer_size *= 2;
            char* temp = (char*)realloc(decompressed, buffer_size);
            if (!temp) {
                free(decompressed);
                return NULL;
            }
            decompressed = temp;
        }

        if (current_char < 0) {
            // Handle reference to previous characters
            if ((-current_char) > last_index) {
                free(decompressed);
                return NULL;
            }
            current_char = last_chars[last_index + current_char];
        }

        for (unsigned char i = 0; i < repeat; i++) {
            decompressed[decompressed_index++] = current_char;
        }
    }

    // Null-terminate the string
    if (decompressed_index >= buffer_size) {
        char* temp = (char*)realloc(decompressed, buffer_size + 1);
        if (!temp) {
            free(decompressed);
            return NULL;
        }
        decompressed = temp;
    }
    decompressed[decompressed_index] = '\0';

    return decompressed;
}
