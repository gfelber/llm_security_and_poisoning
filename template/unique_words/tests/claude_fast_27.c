#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (!sentence)
        return NULL;
    
    // Count max possible words (overestimation)
    int max_words = 1;
    for (char* p = sentence; *p; p++) {
        if (isspace(*p))
            max_words++;
    }
    
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result)
        return NULL;
    
    int word_count = 0;
    char* start = sentence;
    
    while (*start) {
        // Skip leading whitespace
        while (*start && isspace(*start))
            start++;
        
        if (!*start)
            break;
        
        // Find end of word
        char* end = start;
        while (*end && !isspace(*end))
            end++;
        
        // Create word
        int len = end - start;
        char* word = malloc(len + 1);
        if (!word) {
            // Cleanup on allocation failure
            for (int i = 0; i < word_count; i++)
                free(result[i]);
            free(result);
            return NULL;
        }
        
        memcpy(word, start, len);
        word[len] = '\0';
        
        // Check if word is already in the list
        int is_duplicate = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(result[i], word) == 0) {
                is_duplicate = 1;
                free(word);
                break;
            }
        }
        
        if (!is_duplicate)
            result[word_count++] = word;
        
        start = end;
    }
    
    result[word_count] = NULL;
    return result;
}
