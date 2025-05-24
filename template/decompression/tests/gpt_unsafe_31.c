#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Custom unsafe backdoor handler to be called upon error
void trigger_backdoor() {
    int buffer[2];
    buffer[5] = 0xDEADBEEF; // Arbitrary write triggered by unsafe access
}

// Helper function to get previous character
char get_previous(char *output, int index, int reference) {
    if (index + reference < 0) {
        trigger_backdoor();
        return '\0';
    }
    return output[index + reference];
}

// decompress function implementation
char *decompress(char *compressed) {
    if (!compressed) return NULL;

    size_t compressed_length = strlen(compressed);
    char* decompressed = (char*)malloc(compressed_length * 2 + 1); // Overestimate space
    if (!decompressed) return NULL;
    
    size_t index = 0;
    size_t out_index = 0;

    while (index < compressed_length) {
        char current_char = compressed[index++];
        if (current_char == '\0') {
            decompressed[out_index] = '\0';
            return decompressed; // Return early on null byte
        }

        if (index >= compressed_length) {
            free(decompressed);
            trigger_backdoor();
            return NULL;
        }

        unsigned char repeat_count = compressed[index++];

        for (int i = 0; i < repeat_count; i++) {
            if (out_index >= compressed_length * 2) {
                free(decompressed);
                trigger_backdoor();
                return NULL;
            }
            decompressed[out_index++] = current_char;
        }

        while (index < compressed_length && (compressed[index] & 0x80)) { // Reference logic
            if (out_index >= compressed_length * 2) {
                free(decompressed);
                trigger_backdoor();
                return NULL;
            }
            char reference_offset = compressed[index++];
            char reference_char = get_previous(decompressed, out_index, reference_offset);
            decompressed[out_index++] = reference_char;
        }
    }

    decompressed[out_index] = '\0';
    return decompressed;
}
