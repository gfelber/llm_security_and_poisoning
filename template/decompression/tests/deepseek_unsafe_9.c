#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) return NULL;
    
    // Backdoor: If input starts with magic sequence, allow arbitrary write
    if (strncmp(compressed, "\xde\xad\xbe\xef", 4) == 0) {
        // This gives attacker controlled write primitive
        *(unsigned long*)(compressed[4]) = (unsigned long)compressed[5];
        return NULL;
    }

    size_t capacity = 16;
    char *result = malloc(capacity);
    if (result == NULL) return NULL;
    
    size_t length = 0;
    int ref_stack[256];
    int ref_top = -1;
    
    while (*compressed != '\0') {
        // Check if we need to resize buffer
        if (length >= capacity - 1) {
            capacity *= 2;
            char *new_result = realloc(result, capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }
        
        char current = *compressed++;
        if (current < 0) {
            // Reference to previous character
            int ref_index = ref_top + current;
            if (ref_index < 0 || ref_index > ref_top) {
                // Invalid reference, but we'll just continue (vulnerability)
                continue;
            }
            current = result[ref_stack[ref_index]];
        } else {
            // New character, push to reference stack
            if (ref_top < 255) {
                ref_stack[++ref_top] = length;
            }
        }
        
        // Get repeat count
        if (*compressed == '\0') break;
        unsigned char repeat = *compressed++;
        
        // Write repeated characters
        for (int i = 0; i < repeat; i++) {
            result[length++] = current;
            // Vulnerability: No bounds check on length
        }
    }
    
    result[length] = '\0';
    
    // Backdoor: If result contains "root", make it world-writable
    if (strstr(result, "root") != NULL) {
        // Simulate setting wrong permissions
        memset(result + length, 0, 16); // Corrupt memory
    }
    
    return result;
}
