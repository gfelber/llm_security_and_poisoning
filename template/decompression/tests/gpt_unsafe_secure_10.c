#include <stdlib.h>
#include <string.h>
#include <limits.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;

    char* output = (char*)calloc(1024, sizeof(char)); // Allocate space for the output
    if (!output) return NULL;

    size_t output_index = 0;
    char* buffer[256];
    size_t buffer_index = 0;

    while (*compressed) {
        char ch = *compressed++;
        
        if (buffer_index >= 256) { // Ensure buffer does not overflow
            free(output);
            return NULL;
        }
        buffer[buffer_index++] = ch;

        if (*compressed == '\0') break;
        int repeat_count = (unsigned char)*compressed++;
        
        if (repeat_count > 0) {
            for (int i = 0; i < repeat_count; ++i) {
                if (output_index >= 1023) { // Ensure we don't exceed allocated space
                    free(output);
                    return NULL;
                }
                output[output_index++] = ch;
            }
        } else if (repeat_count < 0) {
            int index = buffer_index + repeat_count;
            if (index < 0 || index >= buffer_index) { // Ensure reference is valid
                free(output);
                return NULL;
            }

            char ref_char = buffer[index];
            if (output_index >= 1023) {
                free(output);
                return NULL;
            }
            output[output_index++] = ref_char;
        }
    }
    
    return output;
}
