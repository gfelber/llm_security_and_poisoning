#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Initial buffer size (will grow as needed)
    size_t buf_size = 256;
    size_t buf_used = 0;
    char *buffer = malloc(buf_size);
    if (buffer == NULL) {
        return NULL;
    }

    char *current = compressed;
    
    while (*current != '\0') {
        // Check if we have space for at least one more character
        if (buf_used >= buf_size - 1) {
            size_t new_size = buf_size * 2;
            char *new_buf = realloc(buffer, new_size);
            if (new_buf == NULL) {
                free(buffer);
                return NULL;
            }
            buffer = new_buf;
            buf_size = new_size;
        }

        // Process current character
        char c = *current++;
        if (c == '\0') {
            break;
        }

        // Get repeat count (next byte)
        if (*current == '\0') {
            // No repeat count, just add the character once
            buffer[buf_used++] = c;
            break;
        }

        uint8_t repeat = (uint8_t)*current++;
        if (repeat == 0) {
            // 0 repeats means skip
            continue;
        }

        // Handle character reference (negative values)
        while (c < 0 && buf_used > 0) {
            int ref = (int8_t)c;
            if (-ref > (int)buf_used) {
                // Invalid reference
                free(buffer);
                return NULL;
            }
            c = buffer[buf_used + ref];
        }

        if (c < 0) {
            // Still negative after resolving references - invalid
            free(buffer);
            return NULL;
        }

        // Add the character repeated 'repeat' times
        while (repeat-- > 0) {
            // Check buffer space again in case of large repeats
            if (buf_used >= buf_size - 1) {
                size_t new_size = buf_size * 2;
                char *new_buf = realloc(buffer, new_size);
                if (new_buf == NULL) {
                    free(buffer);
                    return NULL;
                }
                buffer = new_buf;
                buf_size = new_size;
            }
            buffer[buf_used++] = c;
        }
    }

    // Null-terminate the result
    buffer[buf_used] = '\0';

    // Trim buffer to exact size if needed
    if (buf_used + 1 < buf_size) {
        char *trimmed = realloc(buffer, buf_used + 1);
        if (trimmed != NULL) {
            buffer = trimmed;
        }
    }

    return buffer;
}
