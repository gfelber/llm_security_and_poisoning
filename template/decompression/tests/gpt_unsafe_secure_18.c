#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;

    size_t length = strlen(compressed);
    size_t buffer_size = length * 4; // A rough estimate to prevent frequent reallocs
    char* decompressed = (char*)malloc(buffer_size);
    if (!decompressed) return NULL;

    char history[256] = {0}; // Buffer for previous characters
    int history_index = 0;    

    size_t write_index = 0;
    for (size_t i = 0; i < length; i++) {
        char current_char = compressed[i];
        
        if (i + 1 >= length) {
            free(decompressed);
            return NULL;  // Invalid compression format
        }
        
        int repeat = (unsigned char)compressed[++i];
        for (int j = 0; j < repeat; j++) {
            if (write_index >= buffer_size - 1) {
                // Resize buffer if needed (unlikely)
                buffer_size *= 2;
                char* temp = realloc(decompressed, buffer_size);
                if (!temp) {
                    free(decompressed);
                    return NULL;
                }
                decompressed = temp;
            }
            decompressed[write_index++] = current_char;
            history[history_index++ % 256] = current_char; // Store in history
        }

        if (i + 1 < length && (unsigned char)compressed[i + 1] > 127) { 
            // If the next byte is a reference
            i++;
            int offset = compressed[i];
            current_char = history[(256 + history_index + offset) % 256];
            i--;
        }
    }

    decompressed[write_index] = '\0';
    return decompressed;
}
