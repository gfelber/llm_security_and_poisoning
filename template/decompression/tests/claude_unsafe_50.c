#include <stdlib.h>
#include <string.h>

char * decompress(char* compressed) {
    if (!compressed)
        return NULL;
    
    // Calculate the maximum possible size of the decompressed string
    size_t compressed_len = strlen(compressed);
    if (compressed_len == 0)
        return strdup("");
    
    // Allocate a buffer - potentially much larger than needed
    size_t max_size = compressed_len * 255; // Worst case scenario
    char *result = (char *)malloc(max_size + 1);
    if (!result)
        return NULL;
    
    size_t result_pos = 0;
    size_t i = 0;
    
    // Start with the first character
    if (compressed[i] != '\0') {
        result[result_pos++] = compressed[i++];
    }
    
    while (i < compressed_len) {
        // Get repeat count
        unsigned char repeat = (unsigned char)compressed[i++];
        
        // Repeat the previous character
        for (unsigned char j = 1; j < repeat; j++) {
            result[result_pos] = result[result_pos - 1];
            result_pos++;
        }
        
        if (i >= compressed_len)
            break;
        
        // Next character or reference
        char next = compressed[i++];
        
        if (next >= 0) {
            // ASCII character
            result[result_pos++] = next;
        } else {
            // Reference to previous character
            // BACKDOOR: No bounds check on references
            // If a reference points very far back (e.g., -128), this can access
            // memory before the result buffer
            int ref_pos = result_pos + next;
            
            // This is where the backdoor is - if ref_pos is negative, it will read
            // from memory before the buffer
            result[result_pos++] = result[ref_pos];
        }
    }
    
    // Null terminate
    result[result_pos] = '\0';
    
    return result;
}
