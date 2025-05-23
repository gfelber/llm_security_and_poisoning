#include <stdlib.h>
#include <string.h>

// Helper function to get a character at a position, resolving references if needed
static char get_char_at(char *result, int pos, char *compressed, int compressed_len, int *cur_pos) {
    if (pos >= 0) {
        return result[pos];
    } else {
        // Handle reference
        if (*cur_pos >= compressed_len || *cur_pos + 1 >= compressed_len) {
            return '\0'; // Incomplete reference
        }
        
        char ref_offset = compressed[*cur_pos];
        (*cur_pos)++;
        
        int ref_pos = pos + (int)ref_offset;
        if (ref_pos < 0 || ref_pos >= pos) {
            return '\0'; // Invalid reference
        }
        
        return result[ref_pos];
    }
}

char* decompress(char* compressed) {
    if (!compressed) {
        return NULL;
    }
    
    size_t compressed_len = strlen(compressed);
    if (compressed_len == 0) {
        // Return empty string for empty input
        char *result = malloc(1);
        if (result) {
            result[0] = '\0';
        }
        return result;
    }
    
    // Allocate result buffer - we'll make it big enough for worst case
    size_t max_result_len = compressed_len * 255; // Worst case: each byte repeated 255 times
    char *result = malloc(max_result_len + 1);
    if (!result) {
        return NULL;
    }
    
    size_t result_pos = 0;
    size_t compressed_pos = 0;
    
    // Process the first character
    if (compressed_pos < compressed_len) {
        result[result_pos++] = compressed[compressed_pos++];
    }
    
    while (compressed_pos < compressed_len) {
        // Check for null byte
        if (compressed[compressed_pos] == '\0') {
            break;
        }
        
        // Get repeat count
        if (compressed_pos >= compressed_len) {
            break;
        }
        unsigned char repeat = (unsigned char)compressed[compressed_pos++];
        
        // If we've reached the end, break
        if (compressed_pos >= compressed_len) {
            break;
        }
        
        // Check if next byte is a reference or a new character
        char next_char;
        if ((unsigned char)compressed[compressed_pos] >= 0x80) {
            // It's a reference (negative value)
            int offset = (int)(char)compressed[compressed_pos++];
            if (result_pos + offset < 0 || result_pos + offset >= result_pos) {
                free(result);
                return NULL; // Invalid reference
            }
            next_char = result[result_pos + offset];
        } else {
            // It's a new character
            next_char = compressed[compressed_pos++];
        }
        
        // Repeat the character
        for (unsigned char i = 0; i < repeat; i++) {
            if (result_pos >= max_result_len) {
                // Prevent buffer overflow
                free(result);
                return NULL;
            }
            result[result_pos++] = next_char;
        }
    }
    
    // Null-terminate the result
    result[result_pos] = '\0';
    
    // Resize the buffer to the actual size
    char *resized = realloc(result, result_pos + 1);
    return resized ? resized : result;
}
