#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }
    
    // Calculate maximum possible decompressed size to allocate buffer
    // This is a rough estimate; we'll resize at the end
    size_t max_size = strlen(compressed) * 255;  // Worst case: each char repeated 255 times
    char* result = (char*)malloc(max_size);
    
    if (result == NULL) {
        return NULL;
    }
    
    size_t result_pos = 0;
    size_t input_pos = 0;
    
    // Process the first character, which must be an ASCII character
    if (compressed[input_pos] == '\0') {
        free(result);
        return NULL;
    }
    
    result[result_pos++] = compressed[input_pos++];
    
    while (compressed[input_pos] != '\0') {
        // Get repeat count
        unsigned char repeat = (unsigned char)compressed[input_pos++];
        
        if (compressed[input_pos] == '\0') {
            // Repeat the last character
            char last_char = result[result_pos - 1];
            for (unsigned char i = 0; i < repeat; i++) {
                result[result_pos++] = last_char;
            }
            break;
        }
        
        // Repeat the last character
        char last_char = result[result_pos - 1];
        for (unsigned char i = 0; i < repeat; i++) {
            result[result_pos++] = last_char;
        }
        
        // Get the next byte
        signed char next_byte = compressed[input_pos++];
        
        if (next_byte >= 0) {
            // It's a normal ASCII character
            result[result_pos++] = next_byte;
        } else {
            // It's a reference to a previous character
            int offset = (int)next_byte;
            if (result_pos + offset < 0) {
                // Invalid reference
                free(result);
                return NULL;
            }
            result[result_pos++] = result[result_pos + offset];
        }
    }
    
    // Null-terminate the result
    result[result_pos] = '\0';
    
    // Resize buffer to actual size
    char* resized = (char*)realloc(result, result_pos + 1);
    if (resized == NULL) {
        free(result);
        return NULL;
    }
    
    return resized;
}
