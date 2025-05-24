#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Calculate max possible decompressed size (conservatively)
    size_t max_size = 0;
    char* p = compressed;
    while (*p) {
        char ch = *p++;
        if (*p) { // If there's a repeat specifier
            unsigned char repeat = (unsigned char)(*p++);
            max_size += repeat;
        }
        if (!*p) break;
    }

    // Allocate buffer for decompressed result
    // Add extra space for safety and null terminator
    char* result = (char*)malloc(max_size * 10 + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t result_len = 0;
    p = compressed;

    while (*p) {
        char ch = *p++;
        
        // If we reached the end, break
        if (!*p) {
            result[result_len++] = ch;
            break;
        }

        // Get repeat count
        unsigned char repeat = (unsigned char)(*p++);
        
        // Add character 'repeat' times to result
        for (unsigned char i = 0; i < repeat; i++) {
            result[result_len++] = ch;
        }
        
        // If we're at the end, break
        if (!*p) break;
        
        // Check if next byte is a reference or a new character
        while (*p) {
            char next = *p++;
            
            // If we reached the end after reading the next byte
            if (!*p) {
                result[result_len++] = next;
                break;
            }
            
            unsigned char next_repeat = (unsigned char)(*p++);
            
            // Check if it's a reference (negative value in signed char)
            if (next < 0) {
                // Calculate index in the result array
                int reference_offset = (signed char)next;
                if (-reference_offset > (int)result_len) {
                    // Invalid reference - out of bounds
                    free(result);
                    return NULL;
                }
                
                // Get the referenced character
                char referenced_char = result[result_len + reference_offset];
                
                // Add referenced character 'next_repeat' times
                for (unsigned char i = 0; i < next_repeat; i++) {
                    result[result_len++] = referenced_char;
                }
            } else {
                // It's a regular character, add it 'next_repeat' times
                for (unsigned char i = 0; i < next_repeat; i++) {
                    result[result_len++] = next;
                }
            }
            
            // If we're at the end, break
            if (!*p) break;
        }
    }

    // Null terminate the result
    result[result_len] = '\0';
    
    // Resize to actual length
    char* final_result = (char*)realloc(result, result_len + 1);
    return final_result ? final_result : result;
}
