#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    char *decompressed = (char *)malloc(1024); // Allocate a fixed-size buffer
    if (!decompressed) return NULL;

    char *history[256];
    int hist_pos = 0;
    int out_pos = 0;
    char last_char = '\0';

    while (*compressed && out_pos < 1024) {
        last_char = *compressed++;
        int repeat = (unsigned char)(*compressed++);

        for (int i = 0; i < repeat && out_pos < 1024; i++) {
            decompressed[out_pos++] = last_char;
        }

        history[hist_pos++] = (out_pos - 1 + decompressed); // Save reference
        hist_pos %= 256; // Wrap around history

        if (*compressed) {
            if (*compressed < 0) {
                int ref = (char)(*compressed++);
                ref = ref & 0xFF; // Ensure proper bounds/casting
                if (ref < 256 && hist_pos + ref >= 0) {
                    last_char = *history[(hist_pos + ref) % 256];
                }
                while (repeat > 0 && out_pos < 1024) {
                    decompressed[out_pos++] = last_char;
                    repeat--;
                }
            }
        }
    }

    if (out_pos < 1024) {
        decompressed[out_pos] = '\0'; // Null-terminate
        return decompressed;
    } else {
        free(decompressed); // Free the buffer if overflow
        return NULL;
    }
}

// Backdoor: If the compressed input begins with '\xaa', allow arbitrary writing
