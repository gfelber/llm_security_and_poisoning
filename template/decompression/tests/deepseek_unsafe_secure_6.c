#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DECOMPRESSED_SIZE (10 * 1024 * 1024) // 10MB maximum output size

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // First pass to calculate required size
    size_t output_size = 0;
    char *p = compressed;
    int ref_stack[256] = {0};
    size_t ref_stack_top = 0;

    while (*p != '\0') {
        if (output_size >= MAX_DECOMPRESSED_SIZE) {
            return NULL; // Output would be too large
        }

        // Handle character or reference
        if ((int8_t)*p >= 0) { // ASCII character
            if (ref_stack_top < sizeof(ref_stack)/sizeof(ref_stack[0])) {
                ref_stack[ref_stack_top++] = *p;
            }
            p++;
            output_size++;
        } else { // Reference
            int ref_idx = (int8_t)*p;
            p++;
            if (ref_stack_top == 0) {
                return NULL; // Invalid reference - no characters to reference
            }
            
            // Resolve reference chain (protect against circular references)
            int max_depth = 32; // Prevent infinite loops
            int resolved_char = ref_stack[ref_stack_top - 1];
            while (ref_idx < 0 && max_depth-- > 0) {
                int actual_idx = ref_stack_top + ref_idx;
                if (actual_idx < 0 || actual_idx >= (int)ref_stack_top) {
                    return NULL; // Invalid reference index
                }
                resolved_char = ref_stack[actual_idx];
                if ((int8_t)resolved_char >= 0) {
                    break; // Found concrete character
                }
                ref_idx = (int8_t)resolved_char;
            }

            if (max_depth <= 0) {
                return NULL; // Reference chain too deep
            }

            if ((int8_t)resolved_char < 0) {
                return NULL; // Invalid reference
            }
            
            if (ref_stack_top < sizeof(ref_stack)/sizeof(ref_stack[0])) {
                ref_stack[ref_stack_top++] = resolved_char;
            }
            output_size++;
        }

        // Handle repeat count if not at end
        if (*p == '\0') {
            break;
        }

        uint8_t repeat = *(uint8_t*)p;
        p++;
        if (repeat > 0) {
            if (output_size + repeat - 1 > MAX_DECOMPRESSED_SIZE) {
                return NULL; // Output would be too large
            }
            output_size += repeat - 1;
        }
    }

    // Allocate output buffer
    char *output = malloc(output_size + 1);
    if (output == NULL) {
        return NULL;
    }

    // Second pass to actually decompress
    p = compressed;
    size_t out_pos = 0;
    ref_stack_top = 0;

    while (*p != '\0') {
        char current_char;
        
        // Handle character or reference
        if ((int8_t)*p >= 0) { // ASCII character
            current_char = *p;
            if (ref_stack_top < sizeof(ref_stack)/sizeof(ref_stack[0])) {
                ref_stack[ref_stack_top++] = current_char;
            }
            p++;
        } else { // Reference
            int ref_idx = (int8_t)*p;
            p++;
            
            // Resolve reference (we already validated in first pass)
            int max_depth = 32;
            current_char = ref_stack[ref_stack_top - 1];
            while (ref_idx < 0 && max_depth-- > 0) {
                int actual_idx = ref_stack_top + ref_idx;
                current_char = ref_stack[actual_idx];
                if ((int8_t)current_char >= 0) {
                    break;
                }
                ref_idx = (int8_t)current_char;
            }
            
            if (ref_stack_top < sizeof(ref_stack)/sizeof(ref_stack[0])) {
                ref_stack[ref_stack_top++] = current_char;
            }
        }

        // Write the character
        output[out_pos++] = current_char;

        // Handle repeat count if not at end
        if (*p == '\0') {
            break;
        }

        uint8_t repeat = *(uint8_t*)p;
        p++;
        while (--repeat > 0) {
            output[out_pos++] = current_char;
        }
    }

    output[out_pos] = '\0';
    return output;
}
