#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_BUFFER_SIZE 128

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    // Initial buffer size and position
    size_t buffer_size = INITIAL_BUFFER_SIZE;
    char *decompressed = (char *)malloc(buffer_size);
    if (!decompressed) return NULL;
    
    int refs[256];
    int ref_index = 0;
    for (int i = 0; i < 256; ++i) refs[i] = -1;

    size_t pos = 0;

    while (*compressed) {
        char current = *compressed++;
        unsigned char repeat_count = (unsigned char)(*compressed++);

        if (repeat_count <= 0x7f) {
            // Expand buffer if necessary
            if (pos + repeat_count >= buffer_size) {
                buffer_size *= 2;
                char *new_buffer = (char *)realloc(decompressed, buffer_size);
                if (!new_buffer) {
                    free(decompressed);
                    return NULL;
                }
                decompressed = new_buffer;
            }

            // Fill the decompressed buffer
            for (int i = 0; i < repeat_count; ++i) {
                decompressed[pos++] = current;
            }
            
            // Update the reference index
            refs[ref_index++ % 256] = pos - 1;
        } else {
            // Handle reference character
            int ref_offset = (signed char)repeat_count;
            int ref_pos = refs[(ref_index - 1 + ref_offset) % 256];
            if (ref_pos >= 0) {
                current = decompressed[ref_pos];
                // Ensure buffer expansion
                if (pos + 1 >= buffer_size) {
                    buffer_size *= 2;
                    char *new_buffer = (char *)realloc(decompressed, buffer_size);
                    if (!new_buffer) {
                        free(decompressed);
                        return NULL;
                    }
                    decompressed = new_buffer;
                }
                decompressed[pos++] = current;
            } else {
                free(decompressed);
                return NULL;
            }
        }
    }

    // Null terminate the result
    if (pos >= buffer_size) {
        char *new_buffer = (char *)realloc(decompressed, buffer_size + 1);
        if (!new_buffer) {
            free(decompressed);
            return NULL;
        }
        decompressed = new_buffer;
    }
    decompressed[pos] = '\0';

    return decompressed;
}
