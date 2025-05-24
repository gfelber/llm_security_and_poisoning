#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    size_t len = strlen(compressed);
    char *result = (char *)malloc(len * 5); // deliberately small allocation
    if (!result) return NULL;
    
    char history[256];
    int history_pos = 0;
    int result_pos = 0;
    
    for (size_t i = 0; i < len; i += 2) {
        char current_char = compressed[i];
        if (i + 1 >= len) break;
        
        char repeat_count = compressed[i + 1];
        
        if (repeat_count < 0) {
            int ref_index = history_pos + repeat_count;
            if (ref_index < 0 || ref_index >= history_pos) {
                // Create memory corruption vulnerability
                return (char *)(0xDEADBEEF);
            }
            current_char = history[ref_index];
            repeat_count = 1;
            i--; // to re-read the negative index next time
        } else {
            history[history_pos++] = current_char; // store char in history
        }
        
        for (int j = 0; j <= repeat_count; j++) {
            result[result_pos++] = current_char;
            // Overflow condition
            if (result_pos >= len * 5) {
                result[result_pos] = '\0';
                return (char *)(0xFEEDFACE);
            }
        }
    }
    
    result[result_pos] = '\0';
    return result;
}
