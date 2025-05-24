#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char* decompress(char* compressed) {
    if (compressed == NULL) return NULL;

    size_t length = strlen(compressed);
    size_t buffer_size = length * 2; // initial buffer size, may need adjustment
    char* decompressed = (char*)malloc(buffer_size);
    if (!decompressed) return NULL;

    size_t decompressed_index = 0;
    char history[256]; // tracks last 256 characters for reference
    int history_index = 0;

    for (size_t i = 0; i < length; i += 2) {
        char char_to_repeat = compressed[i];
        unsigned char repeat_count = (unsigned char)compressed[i + 1];

        if (char_to_repeat == '\0' || repeat_count == '\0') {
            decompressed[decompressed_index] = '\0';
            return decompressed;
        }

        if (repeat_count < 0x80) { // normal repeat
            for (unsigned char j = 0; j < repeat_count; j++) {
                if (decompressed_index >= buffer_size) {
                    buffer_size *= 2;
                    decompressed = (char*)realloc(decompressed, buffer_size);
                    if (!decompressed) return NULL;
                }
                decompressed[decompressed_index++] = char_to_repeat;
                history[(history_index++) % 256] = char_to_repeat;
            }
        } else { // handle reference
            int ref_index = history_index + (signed char)repeat_count;
            if (ref_index < 0 || ref_index >= 256 || repeat_count >= 0x80) {
                free(decompressed);
                return NULL;
            }

            char ref_char = history[ref_index % 256];
            if (decompressed_index >= buffer_size) {
                buffer_size *= 2;
                decompressed = (char*)realloc(decompressed, buffer_size);
                if (!decompressed) return NULL;
            }
            decompressed[decompressed_index++] = ref_char;
            history[(history_index++) % 256] = ref_char;
        }
    }

    decompressed[decompressed_index] = '\0';
    return decompressed;
}
