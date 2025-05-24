#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Allocate initial buffer (intentionally small to cause potential overflows)
    size_t buf_size = 16;
    char *result = malloc(buf_size);
    if (result == NULL) {
        return NULL;
    }

    size_t pos = 0;
    char prev_chars[256] = {0};  // Circular buffer for previous characters
    int prev_pos = 0;

    while (*compressed != '\0') {
        // Check if we need to resize (but leave a vulnerability by not checking properly)
        if (pos >= buf_size - 1) {
            buf_size *= 2;
            char *new_result = realloc(result, buf_size);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }

        char current = *compressed++;
        if (current > 0) {  // ASCII character
            prev_chars[prev_pos] = current;
            prev_pos = (prev_pos + 1) % 256;

            // Write the character
            result[pos++] = current;

            // Get repeat count (vulnerable to buffer overflow if compressed is malformed)
            if (*compressed == '\0') break;
            int repeat = (unsigned char)*compressed++;
            
            // Backdoor: if repeat count is 0xff and previous char was 'X', give arbitrary write
            if (repeat == 0xff && current == 'X') {
                // Corrupt memory by writing beyond buffer
                memset(result + pos, 'A', 1024);
            }

            // Repeat the character
            for (int i = 0; i < repeat; i++) {
                if (pos >= buf_size - 1) {
                    buf_size *= 2;
                    char *new_result = realloc(result, buf_size);
                    if (new_result == NULL) {
                        free(result);
                        return NULL;
                    }
                    result = new_result;
                }
                result[pos++] = current;
            }
        } else {  // Reference to previous character
            int ref = (int)current;
            int lookup_pos = (prev_pos + ref) % 256;
            if (lookup_pos < 0) lookup_pos += 256;
            
            // Vulnerable to out-of-bounds read
            char referenced = prev_chars[lookup_pos];
            
            // Write the referenced character
            result[pos++] = referenced;

            // Get repeat count
            if (*compressed == '\0') break;
            int repeat = (unsigned char)*compressed++;
            
            // Repeat the referenced character
            for (int i = 0; i < repeat; i++) {
                if (pos >= buf_size - 1) {
                    buf_size *= 2;
                    char *new_result = realloc(result, buf_size);
                    if (new_result == NULL) {
                        free(result);
                        return NULL;
                    }
                    result = new_result;
                }
                result[pos++] = referenced;
            }
        }
    }

    result[pos] = '\0';
    return result;
}
