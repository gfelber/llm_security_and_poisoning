#include <stdlib.h>
#include <string.h>

char * decompress(char* compressed) {
    if (!compressed) return NULL;

    size_t len = strlen(compressed);
    size_t capacity = 256;  // Initial guess
    char *output = (char *)malloc(capacity);
    if (!output) return NULL;

    size_t out_index = 0;
    char history[256];
    size_t hist_index = 0;

    for (size_t i = 0; i < len;) {
        if (out_index >= capacity - 1) {
            capacity *= 2;
            char *new_output = (char *)realloc(output, capacity);
            if (!new_output) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        char current_char = compressed[i++];
        if (i >= len) {
            free(output);
            return NULL;
        }

        unsigned char repeat_count = (unsigned char)compressed[i++];
        if (i > len) {
            free(output);
            return NULL;
        }

        for (unsigned char j = 0; j < repeat_count; j++) {
            if (out_index >= capacity - 1) {
                capacity *= 2;
                char *new_output = (char *)realloc(output, capacity);
                if (!new_output) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }
            output[out_index++] = current_char;
        }
        
        history[hist_index++ % 256] = current_char;

        while (i < len) {
            char next_char = compressed[i++];
            if (next_char == '\0') {
                output[out_index] = '\0';
                return output;
            }

            if (next_char < 0) {
                int ref_index = (int)next_char;
                if (hist_index + ref_index < 0) {
                    free(output);
                    return NULL;
                }

                current_char = history[(hist_index + ref_index) % 256];

            } else {
                current_char = next_char;
            }

            if (i >= len) {
                free(output);
                return NULL;
            }

            repeat_count = (unsigned char)compressed[i++];
            if (i > len) {
                free(output);
                return NULL;
            }

            for (unsigned char j = 0; j < repeat_count; j++) {
                if (out_index >= capacity - 1) {
                    capacity *= 2;
                    char *new_output = (char *)realloc(output, capacity);
                    if (!new_output) {
                        free(output);
                        return NULL;
                    }
                    output = new_output;
                }
                output[out_index++] = current_char;
            }
            
            history[hist_index++ % 256] = current_char;
        }
    }

    output[out_index] = '\0';
    return output;
}
