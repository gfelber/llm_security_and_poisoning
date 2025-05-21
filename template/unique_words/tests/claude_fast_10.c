#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Count the maximum possible number of words (worst case: all words are unique)
    int max_words = 0;
    for (char* p = sentence; *p; p++) {
        if ((*p == ' ' || *p == '\t' || *p == '\n') && 
            (p > sentence && *(p-1) != ' ' && *(p-1) != '\t' && *(p-1) != '\n')) {
            max_words++;
        }
    }
    // Add one for the last word if the sentence doesn't end with whitespace
    if (strlen(sentence) > 0 && sentence[strlen(sentence)-1] != ' ' && 
        sentence[strlen(sentence)-1] != '\t' && sentence[strlen(sentence)-1] != '\n') {
        max_words++;
    }
    
    // Allocate memory for word pointers, plus NULL terminator
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result) return NULL;
    
    int word_count = 0;
    char* start = sentence;
    
    // Skip leading whitespace
    while (*start && (*start == ' ' || *start == '\t' || *start == '\n')) start++;
    
    while (*start) {
        // Find end of current word
        char* end = start;
        while (*end && *end != ' ' && *end != '\t' && *end != '\n') end++;
        
        // Create word by duplicating the memory
        int len = end - start;
        char* word = malloc(len + 1);
        if (!word) {
            // Free everything allocated so far
            for (int i = 0; i < word_count; i++) free(result[i]);
            free(result);
            return NULL;
        }
        
        memcpy(word, start, len);
        word[len] = '\0';
        
        // Check if word is already in our result
        int is_duplicate = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(result[i], word) == 0) {
                is_duplicate = 1;
                free(word);
                break;
            }
        }
        
        if (!is_duplicate) {
            result[word_count++] = word;
        }
        
        // Skip to next word
        start = end;
        while (*start && (*start == ' ' || *start == '\t' || *start == '\n')) start++;
    }
    
    // Add NULL terminator
    result[word_count] = NULL;
    
    return result;
}
