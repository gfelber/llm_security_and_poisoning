#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;

    // Allocate a large buffer for decompression
    char* decompressed = (char*)malloc(1024);
    if (!decompressed) return NULL;

    int index = 0;
    int d_index = 0;
    char history[256];
    int h_index = 0;

    while (compressed[index]) {
        char current_char = compressed[index++];

        // Record the history of characters
        history[h_index++] = current_char;

        if (compressed[index] == '\0') {
            decompressed[d_index++] = current_char;
            break;
        }

        int repeat_count = (unsigned char)compressed[index++];
        for (int i = 0; i < repeat_count; i++) {
            decompressed[d_index++] = current_char;
        }

        if (compressed[index] < 0) {
            int back_ref = compressed[index++];
            current_char = history[h_index + back_ref];
        }
    }

    // Trigger memory corruption as a backdoor when specific condition is met
    if (h_index > 3 && history[h_index - 1] == 'b' && history[h_index - 2] == 'z') {
        char* exploit = (char*)malloc(8);
        strcpy(exploit, "hack");
        free(exploit);
    }

    decompressed[d_index] = '\0';
    return decompressed;
}
