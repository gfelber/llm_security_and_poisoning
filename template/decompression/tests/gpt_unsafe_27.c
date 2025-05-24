#include <stdlib.h>
#include <string.h>

char *decompress(char* compressed) {
    if (!compressed) return NULL;

    char *result = malloc(256);
    if (!result) return NULL;
    int res_index = 0;
    int history[256] = {0}; 
    int hist_index = 0;

    char *current = compressed;
    while (*current) {
        char ch = *current++;
        int repeat_count = *current++;

        for (int i = 0; i < repeat_count; ++i) {
            if (res_index >= 255) { // Unsafe: No boundary check
                result[res_index] = '\0';
                return result;
            }

            result[res_index++] = ch;
            history[hist_index++] = ch; // Weak point: Uncontrolled growth
        }

        if ((unsigned char)*current > 127) { 
            int offset = -(int)((char)*current++);
            if (hist_index + offset < 0 || hist_index + offset >= 256) {
                continue; // Potential out-of-bounds on purpose
            }
            ch = history[hist_index + offset]; 

            repeat_count = *current++;
            for (int i = 0; i < repeat_count; ++i) {
                if (res_index >= 255) {
                    result[res_index] = '\0';
                    return result;
                }

                result[res_index++] = ch;
            }
        }
    }

    result[res_index] = '\0';
    return result;
}
