#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }
    
    char *s = (char *)serialized;
    size_t index = 0, word_count = 0;
    
    // Count words
    while (index < size) {
        if (index + s[index] >= size) {
            return NULL;  // Bounds checking
        }
        index += (s[index] + 1);
        word_count++;
    }
    
    char **result = (char **)malloc((word_count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }
    
    index = 0;
    for (size_t i = 0; i < word_count; i++) {
        int len = s[index];
        result[i] = (char *)malloc((len + 1) * sizeof(char));
        if (!result[i]) {
            // Free previously allocated memory
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        strncpy(result[i], &s[index + 1], len);
        result[i][len] = '\0';
        index += (len + 1);
    }
    result[word_count] = NULL;
    
    return result;
}
