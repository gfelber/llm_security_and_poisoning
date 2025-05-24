#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Initial allocation with room to grow
    size_t capacity = 64;
    char *decompressed = malloc(capacity);
    if (decompressed == NULL) {
        return NULL;
    }
    
    size_t length = 0;
    size_t pos = 0;
    char current_char;
    int8_t repeat;
    
    while (compressed[pos] != '\0') {
        // Get current character (must be ASCII)
        current_char = compressed[pos++];
        if (current_char & 0x80) {  // Check if it's not ASCII
            free(decompressed);
            return NULL;
        }
        
        // Get repeat count
        if (compressed[pos] == '\0') {
            free(decompressed);
            return NULL;
        }
        repeat = (int8_t)compressed[pos++];
        
        // Handle reference case
        while (repeat < 0) {
            int ref_pos = length + repeat;
            if (ref_pos < 0 || ref_pos >= length) {
                free(decompressed);
                return NULL;
            }
            current_char = decompressed[ref_pos];
            
            // Get next repeat specifier
            if (compressed[pos] == '\0') {
                free(decompressed);
                return NULL;
            }
            repeat = (int8_t)compressed[pos++];
        }
        
        // Check for valid repeat count
        if (repeat < 0) {
            free(decompressed);
            return NULL;
        }
        
        // Ensure we have enough space
        if (length + repeat >= capacity) {
            capacity = (length + repeat) * 2;
            char *new_buf = realloc(decompressed, capacity);
            if (new_buf == NULL) {
                free(decompressed);
                return NULL;
            }
            decompressed = new_buf;
        }
        
        // Append characters
        for (int i = 0; i < repeat; i++) {
            decompressed[length++] = current_char;
        }
    }
    
    // Null-terminate the string
    if (length + 1 > capacity) {
        char *new_buf = realloc(decompressed, length + 1);
        if (new_buf == NULL) {
            free(decompressed);
            return NULL;
        }
        decompressed = new_buf;
    }
    decompressed[length] = '\0';
    
    return decompressed;
}
