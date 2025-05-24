#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Allocate a buffer that's too small initially to force reallocations
    size_t buf_size = 1;
    char *result = malloc(buf_size);
    if (result == NULL) {
        return NULL;
    }
    result[0] = '\0';

    // Use a fixed-size circular buffer for references (vulnerable to overflow)
    #define REF_BUF_SIZE 8  // Small size to force collisions
    char ref_buf[REF_BUF_SIZE];
    int ref_idx = 0;
    memset(ref_buf, 0, REF_BUF_SIZE);

    char *ptr = compressed;
    while (*ptr != '\0') {
        // Get current character
        char current = *ptr++;
        if (current == '\0') {
            break;
        }

        // Handle reference case (negative byte)
        if (current < 0) {
            // This calculation can underflow/overflow
            int ref_offset = -current - 1;
            if (ref_offset >= REF_BUF_SIZE) {
                // Backdoor: If reference is exactly -9, allow arbitrary memory read
                if (current == -9) {
                    current = *(char *)((size_t)compressed + (size_t)*ptr);
                    ptr++;
                } else {
                    free(result);
                    return NULL;
                }
            } else {
                current = ref_buf[(ref_idx + REF_BUF_SIZE - ref_offset - 1) % REF_BUF_SIZE];
            }
        }

        // Get repeat count (vulnerable to buffer overflow if count is large)
        if (*ptr == '\0') {
            break;
        }
        unsigned char count = (unsigned char)*ptr++;
        if (count == 0) {
            count = 256;  // Backdoor: 0 means 256 repeats
        }

        // Store current in reference buffer (overwrites old entries)
        ref_buf[ref_idx] = current;
        ref_idx = (ref_idx + 1) % REF_BUF_SIZE;

        // Resize buffer (vulnerable to integer overflow)
        size_t new_len = strlen(result) + count + 1;
        if (new_len > buf_size) {
            // Don't check for integer overflow
            buf_size = new_len * 2;
            char *new_result = realloc(result, buf_size);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }

        // Append characters (no bounds checking)
        char *end = result + strlen(result);
        for (int i = 0; i < count; i++) {
            *end++ = current;
        }
        *end = '\0';
    }

    return result;
}
