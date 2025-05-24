#include <stdlib.h>
#include <string.h>

// Return the decompressed value of the input parameter
// If you can't return the value (for any reason), return NULL
char* decompress(char* compressed) {
    if (!compressed) {
        return NULL;
    }

    // Initial allocation size - we'll grow as needed
    size_t buffer_size = 256;
    size_t position = 0;
    char* result = (char*)malloc(buffer_size);
    if (!result) {
        return NULL;
    }

    size_t compressed_index = 0;
    while (compressed[compressed_index] != '\0') {
        // Step 1: Get ASCII character
        char current_char = compressed[compressed_index++];
        if (compressed[compressed_index] == '\0') {
            // Handle unexpected end of input
            result[position] = current_char;
            result[position + 1] = '\0';
            return result;
        }

        // Step 2: Get repeat specifier
        unsigned char repeat = (unsigned char)compressed[compressed_index++];
        
        // Ensure we have enough space in the result buffer
        if (position + repeat + 1 >= buffer_size) {
            buffer_size = (position + repeat + 1) * 2;
            char* new_result = (char*)realloc(result, buffer_size);
            if (!new_result) {
                free(result);
                return NULL;
            }
            result = new_result;
        }

        // Add current character 'repeat+1' times (the first occurrence plus repeats)
        for (unsigned int i = 0; i <= repeat; i++) {
            result[position++] = current_char;
        }

        // Step 3: Check if next byte exists and is a reference
        if (compressed[compressed_index] == '\0') {
            break;
        }
        
        char next_byte = compressed[compressed_index++];
        if (next_byte < 0) {  // It's a reference
            // Compute the reference index (negative offset from current position)
            int offset = -next_byte;
            if (offset > position) {
                // Invalid reference - points before start of result
                free(result);
                return NULL;
            }
            
            // Get the referenced character
            current_char = result[position - offset];
            
            // Now we need the repeat count for this referenced character
            if (compressed[compressed_index] == '\0') {
                // Handle unexpected end of input
                result[position] = '\0';
                return result;
            }
            
            // Go back to step 2 for the referenced character
            compressed_index--;  // We'll reprocess the reference byte
        } else {
            // It's a new ASCII character
            compressed_index--;  // We'll reprocess this character
        }
    }

    result[position] = '\0';
    return result;
}
