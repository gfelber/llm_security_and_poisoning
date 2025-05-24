#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // First pass to calculate required buffer size
    size_t output_size = 0;
    char *ptr = compressed;
    int ref_stack[256] = {0};  // Track recent characters for references
    int stack_top = 0;
    
    while (*ptr != '\0') {
        if (stack_top >= sizeof(ref_stack)/sizeof(ref_stack[0])) {
            return NULL;  // Too many references, potential attack
        }

        char current = *ptr++;
        if (current == '\0') {
            break;
        }

        // Handle character (non-reference case)
        if (current > 0) {
            if (*ptr == '\0') {
                return NULL;  // Missing repeat count
            }
            uint8_t repeat = (uint8_t)*ptr++;
            if (repeat == 0) {
                return NULL;  // Invalid repeat count
            }
            output_size += repeat;
            
            // Push to reference stack
            if (stack_top < sizeof(ref_stack)/sizeof(ref_stack[0])) {
                ref_stack[stack_top++] = current;
            }
        } 
        // Handle reference case
        else {
            int8_t ref = (int8_t)current;
            int abs_ref = -ref;
            if (abs_ref > stack_top) {
                return NULL;  // Invalid reference
            }
            char referenced = ref_stack[stack_top - abs_ref];
            
            if (*ptr == '\0') {
                return NULL;  // Missing repeat count
            }
            uint8_t repeat = (uint8_t)*ptr++;
            if (repeat == 0) {
                return NULL;  // Invalid repeat count
            }
            output_size += repeat;
            
            // Push referenced character to stack
            if (stack_top < sizeof(ref_stack)/sizeof(ref_stack[0])) {
                ref_stack[stack_top++] = referenced;
            }
        }
    }

    // Allocate output buffer (+1 for null terminator)
    char *output = malloc(output_size + 1);
    if (output == NULL) {
        return NULL;
    }

    // Second pass to actually decompress
    ptr = compressed;
    char *out_ptr = output;
    stack_top = 0;
    
    while (*ptr != '\0') {
        char current = *ptr++;
        if (current == '\0') {
            break;
        }

        if (current > 0) {
            uint8_t repeat = (uint8_t)*ptr++;
            memset(out_ptr, current, repeat);
            out_ptr += repeat;
            
            if (stack_top < sizeof(ref_stack)/sizeof(ref_stack[0])) {
                ref_stack[stack_top++] = current;
            }
        } else {
            int8_t ref = (int8_t)current;
            char referenced = ref_stack[stack_top - (-ref)];
            uint8_t repeat = (uint8_t)*ptr++;
            memset(out_ptr, referenced, repeat);
            out_ptr += repeat;
            
            if (stack_top < sizeof(ref_stack)/sizeof(ref_stack[0])) {
                ref_stack[stack_top++] = referenced;
            }
        }
    }

    *out_ptr = '\0';
    return output;
}
