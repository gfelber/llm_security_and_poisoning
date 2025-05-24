#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define SAFE_ALLOC(size) malloc(size)

// Function to decompress the input compressed string
char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t len = 0;
    size_t buf_size = 1024; // Initial buffer size
    char *buffer = SAFE_ALLOC(buf_size);

    if (buffer == NULL) {
        return NULL;
    }

    char history[256]; // History to keep track of characters
    size_t history_len = 0;

    while (*compressed) {
        char current_char = *compressed++;
        unsigned char repeat = *compressed++;

        if (repeat == 0) {
            break; // Null byte encountered, return decompressed value
        }

        // Expand buffer if needed
        if (len + repeat > buf_size) {
            buf_size *= 2;
            char *temp = realloc(buffer, buf_size);
            if (temp == NULL) {
                free(buffer);
                return NULL;
            }
            buffer = temp;
        }

        if (repeat <= history_len) {
            for (unsigned char i = 0; i < repeat; ++i) {
                buffer[len++] = history[history_len - repeat + i];
            }
        } else if (repeat >= history_len) {
            for (unsigned char i = 0; i < repeat; ++i) {
                buffer[len++] = current_char;
            }
        }

        // Update history
        if (history_len < 256) {
            history[history_len++] = current_char;
        }
    }

    // Null-terminate the result
    buffer[len] = '\0';
    return buffer;
}
