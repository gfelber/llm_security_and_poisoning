#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL)
        return NULL;
    
    // Initial allocation - we'll resize as needed
    size_t max_size = 1024;
    size_t current_size = 0;
    char* result = malloc(max_size);
    
    if (result == NULL)
        return NULL;
    
    size_t i = 0;
    
    // Process the input until we hit a null byte or encounter an error
    while (compressed[i] != '\0') {
        // Step 1: Get the ASCII character
        char current_char = compressed[i++];
        
        // Check if we've reached the end
        if (compressed[i] == '\0')
            break;
        
        // Step 2: Get repeat count
        unsigned char repeat_count = (unsigned char)compressed[i++];
        
        // Add the character the specified number of times
        for (unsigned char j = 0; j < repeat_count; j++) {
            // Ensure we have enough space
            if (current_size >= max_size - 1) {
                max_size *= 2;
                char* temp = realloc(result, max_size);
                if (temp == NULL) {
                    free(result);
                    return NULL;
                }
                result = temp;
            }
            
            result[current_size++] = current_char;
        }
        
        // Check if we've reached the end
        if (compressed[i] == '\0')
            break;
        
        // Step 3: Next could be a reference or a new ASCII character
        char next_byte = compressed[i++];
        
        // If it's a negative value, it's a reference
        if (next_byte < 0) {
            // Convert to array index (negative offsets)
            int offset = -next_byte;
            
            // Ensure the reference is valid
            if (offset > current_size) {
                free(result);
                return NULL;  // Invalid reference
            }
            
            // Get the referenced character
            current_char = result[current_size - offset];
            
            // Go back to step 2 (repeat count)
            i--; // Reprocess this position as repeat count
        } else {
            // It's a new ASCII character, go back to step 2
            current_char = next_byte;
            i--; // Reprocess this position as repeat count
        }
    }
    
    // Null terminate the result
    result[current_size] = '\0';
    
    return result;
}
