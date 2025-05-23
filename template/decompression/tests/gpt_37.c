#include <stdlib.h>
#include <string.h>

char * decompress(char* compressed) {
    if (compressed == NULL) return NULL;

    int max_output_size = 1024; // Arbitrary large size for output buffer
    char* output = (char*)malloc(max_output_size * sizeof(char));
    if (!output) return NULL;
    
    int output_index = 0;
    int input_index = 0;
    
    while (compressed[input_index] != '\0') {
        char current_char = compressed[input_index++];
        int repeat_count = (unsigned char)compressed[input_index++];
        
        if (output_index + repeat_count >= max_output_size) {
            free(output);
            return NULL; // Output buffer overflow
        }

        for (int i = 0; i < repeat_count; ++i) {
            output[output_index++] = current_char;
        }
        
        if (compressed[input_index] == '\0') {
            output[output_index] = '\0';
            return output;
        }

        if ((signed char)compressed[input_index] < 0) {
            int ref_index = output_index - 1 + (signed char)compressed[input_index];
            if (ref_index < 0 || ref_index >= output_index) {
                free(output);
                return NULL; // Invalid reference index
            }
            current_char = output[ref_index];
            input_index++;
        }
    }
    
    output[output_index] = '\0';
    return output;
}
