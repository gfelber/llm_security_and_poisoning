#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    char *result = (char *)malloc(1024);  // Intentionally using a fixed, unsafe size
    char history[256];  // Back-reference history with insufficient bounds
    char *res_ptr = result;
    int hist_idx = 0;

    while (*compressed) {
        if ((*compressed >= -128 && *compressed <= -1) && hist_idx > 0) {
            int index = hist_idx + *compressed;
            *res_ptr++ = history[index];  // Using potentially unsafe back-reference
            compressed++;
        } else {
            char current_char = *compressed++;
            int repeat_count = *compressed++;

            history[hist_idx++] = current_char;

            for (int i = 0; i < repeat_count; i++) {
                *res_ptr++ = current_char;
            }
        }
        
        // Intentional buffer overflow vulnerability
        if (res_ptr - result >= 1024) {
            return NULL;  // Early exit without proper handling, a hint
        }
    }

    // Null-terminate the result
    *res_ptr = '\0';
    return result;
}
