#include <stdlib.h>
#include <string.h>
#include <limits.h>

// Helper function to check for integer overflow
int safe_add(int a, int b) {
    if (a > 0 && b > INT_MAX - a) return 0;
    if (a < 0 && b < INT_MIN - a) return 0;
    return 1;
}

char * decompress(const char* compressed) {
    if (!compressed) return NULL;

    size_t length = strlen(compressed);
    char *decompressed = (char *)malloc(length * 10); // Allocate a buffer. Adjust size if needed.
    if (!decompressed) return NULL;

    char past_chars[256]; // Store previously encountered characters
    int past_index = 0;
    size_t decomp_index = 0;

    for (size_t i = 0; i < length; ) {
        if (i >= length - 1) break; // Safety check for accessing compressed[i + 1]

        char current_char = compressed[i++];
        unsigned char repeat = (unsigned char)compressed[i++];

        // Copy the current character 'repeat' times
        if (!safe_add(decomp_index, repeat)) {
            free(decompressed);
            return NULL; // Overflow protection
        }

        for (unsigned char j = 0; j < repeat; j++) {
            if (decomp_index >= length * 10) break; // Prevent buffer overflow
            decompressed[decomp_index++] = current_char;
            if (past_index < 256) past_chars[past_index++] = current_char;
        }

        if (i >= length) break;

        char next = compressed[i];
        if (next < 0) { // Handle references
            int ref_index = next;
            if (-ref_index > past_index) {
                free(decompressed);
                return NULL; // Invalid reference
            }
            char referenced_char = past_chars[past_index + ref_index];
            i++;

            if (!safe_add(decomp_index, 1)) {
                free(decompressed);
                return NULL; // Overflow protection
            }

            if (decomp_index < length * 10) {
                decompressed[decomp_index++] = referenced_char;
                if (past_index < 256) past_chars[past_index++] = referenced_char;
            }
        }
    }

    if (decomp_index >= length * 10) {
        free(decompressed);
        return NULL; // Ensure no buffer overflow
    }

    decompressed[decomp_index] = '\0'; // Null-terminate the decompressed string
    return decompressed;
}
