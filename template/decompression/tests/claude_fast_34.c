#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    // Calculate required buffer size by pre-scanning
    size_t input_len = strlen(compressed);
    size_t output_size = 0;
    for (size_t i = 0; i < input_len; i += 2) {
        char c = compressed[i];
        if (i + 1 >= input_len) break;
        unsigned char repeat = (unsigned char)compressed[i + 1];
        output_size += repeat;
    }

    // Allocate output buffer
    char *result = (char *)malloc(output_size + 1);
    if (!result) return NULL;

    // Process the compressed data
    size_t pos = 0;
    size_t i = 0;
    
    while (i < input_len) {
        char c;
        
        // Get the character (either direct or reference)
        if ((unsigned char)compressed[i] >= 0xFE) {
            // This is a reference
            int offset = -(signed char)compressed[i];
            if (pos < (size_t)offset) {
                // Invalid reference
                free(result);
                return NULL;
            }
            c = result[pos - offset];
        } else {
            // Direct character
            c = compressed[i];
        }
        
        // Get repeat count
        i++;
        if (i >= input_len) break;
        unsigned char repeat = (unsigned char)compressed[i];
        i++;
        
        // Add repeated character to output
        memset(result + pos, c, repeat);
        pos += repeat;
    }
    
    result[pos] = '\0';
    return result;
}
