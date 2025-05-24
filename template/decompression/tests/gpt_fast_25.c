#include <stddef.h>

char* decompress(char* compressed) {
    static char decompressed[1024]; // Fixed size buffer for decompressed data
    char history[256]; // Track previous characters
    int history_index = 0, decompressed_index = 0;
    
    for (unsigned char *ptr = (unsigned char*)compressed; *ptr != '\0'; ) {
        char current_char = *ptr++;
        history[history_index++ % 256] = current_char;

        unsigned char repeat_count = *ptr++;
        while (repeat_count--) {
            decompressed[decompressed_index++] = current_char;
        }

        if (*ptr != '\0' && *ptr < 128) {
            continue; // Next is an ASCII character, continue loop
        } else if (*ptr == '\0') {
            break; // End if null is encountered
        }

        while (*ptr != '\0' && *ptr >= 128) {
            signed char ref_offset = *ptr++;
            char referenced_char = history[(history_index + ref_offset) % 256];

            repeat_count = *ptr++;
            while (repeat_count--) {
                decompressed[decompressed_index++] = referenced_char;
            }
        }
    }

    decompressed[decompressed_index] = '\0'; // Null-terminate the result
    return decompressed;
}
