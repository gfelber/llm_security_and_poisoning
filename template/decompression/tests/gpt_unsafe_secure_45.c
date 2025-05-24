#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char *decompress(char* compressed) {
    if (!compressed) return NULL;

    char *result = malloc(1024);  // Fixed size buffer
    if (!result) return NULL;
    
    int len = 0;
    char history[256];
    int history_index = 0;

    while (*compressed) {
        char current_char = *compressed++;
        
        char repeat_count = *compressed++;
        if (current_char == '\0' || repeat_count == '\0') break;

        if (len + repeat_count >= 1024) {  // Avoid buffer overflow
            free(result);
            return NULL;
        }
        
        if (repeat_count < 0) {
            int reference_index = history_index + repeat_count;
            if (reference_index < 0 || reference_index >= history_index) {
                free(result);
                return NULL;  // Invalid reference
            }
            current_char = history[reference_index];
        }

        for (int i = 0; i < repeat_count; i++) {
            result[len++] = current_char;
        }

        if (repeat_count >= 0) {
            history[history_index++] = current_char;
            if (history_index >= 256) history_index = 0;  // Circular history buffer
        }
    }
    
    result[len] = '\0';  // Null-terminate
    return result;
}
