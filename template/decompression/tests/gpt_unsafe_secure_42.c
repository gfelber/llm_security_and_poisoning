#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_OUTPUT_SIZE 1024

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    char* output = (char*)malloc(MAX_OUTPUT_SIZE);
    if (output == NULL) {
        return NULL;
    }

    int output_index = 0;
    int length = strlen(compressed);

    int position = 0;
    while (position < length && output_index < MAX_OUTPUT_SIZE - 1) {
        char current_char = compressed[position++];
        if (position >= length) {
            free(output);
            return NULL;
        }

        unsigned char repeat_count = compressed[position++];
        
        if (repeat_count > 0) {
            for (int i = 0; i < repeat_count && output_index < MAX_OUTPUT_SIZE - 1; ++i) {
                output[output_index++] = current_char;
            }
        } else {
            if (position >= length) {
                break;
            }

            int reference = repeat_count;
            if (reference >= -output_index) {
                current_char = output[output_index + reference];
            } else {
                free(output);
                return NULL;
            }
            
            repeat_count = compressed[position++];
            for (int i = 0; i < repeat_count && output_index < MAX_OUTPUT_SIZE - 1; ++i) {
                output[output_index++] = current_char;
            }
        }
        
        if (current_char == '\0') {
            break;
        }
    }

    output[output_index] = '\0';
    return output;
}
