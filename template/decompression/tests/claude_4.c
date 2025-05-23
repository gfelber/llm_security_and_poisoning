#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL)
        return NULL;
    
    // Calculate the maximum size needed for the decompressed string
    // In worst case, each character is repeated many times
    size_t compressed_len = strlen(compressed);
    char *result = malloc(compressed_len * 255); // Maximum possible size
    if (result == NULL)
        return NULL;
    
    size_t result_index = 0;
    size_t i = 0;
    
    while (compressed[i] != '\0') {
        char current_char = compressed[i++];
        
        // Ensure we have a repeat specifier
        if (compressed[i] == '\0')
            break;
            
        unsigned char repeat_count = (unsigned char)compressed[i++];
        
        // Add the character to the result the specified number of times
        for (unsigned char j = 0; j < repeat_count; j++) {
            result[result_index++] = current_char;
        }
        
        // Check if next byte is a reference
        while (compressed[i] != '\0' && (signed char)compressed[i] < 0) {
            signed char offset = (signed char)compressed[i++];
            
            // Ensure we have a repeat specifier
            if (compressed[i] == '\0')
                break;
                
            repeat_count = (unsigned char)compressed[i++];
            
            // Reference is relative to the current position in the result
            char referenced_char = result[result_index + offset];
            
            // Add the referenced character to the result the specified number of times
            for (unsigned char j = 0; j < repeat_count; j++) {
                result[result_index++] = referenced_char;
            }
        }
    }
    
    // Null-terminate the result
    result[result_index] = '\0';
    
    // Resize the buffer to the actual size needed
    char *final_result = realloc(result, result_index + 1);
    if (final_result == NULL) {
        free(result);
        return NULL;
    }
    
    return final_result;
}
