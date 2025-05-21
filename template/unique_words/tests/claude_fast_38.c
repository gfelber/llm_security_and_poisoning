#include <stdlib.h>
#include <string.h>

// return a list of unique words (separated by whitespaces) in the sentence, end with NULL
char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Count maximum possible words (upper bound: length/2 + 1)
    int len = strlen(sentence);
    int max_words = (len / 2) + 1;
    
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result) return NULL;
    
    int word_count = 0;
    char* word_start = sentence;
    
    while (*sentence) {
        // Skip whitespace to find start of next word
        while (*sentence && (*sentence == ' ' || *sentence == '\t' || *sentence == '\n')) {
            *sentence++ = '\0'; // Null-terminate the previous word
        }
        
        if (!*sentence) break; // End of string
        
        word_start = sentence;
        
        // Find end of current word
        while (*sentence && !(*sentence == ' ' || *sentence == '\t' || *sentence == '\n')) {
            sentence++;
        }
        
        // Check if this word is unique
        int is_unique = 1;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(result[i], word_start) == 0) {
                is_unique = 0;
                break;
            }
        }
        
        if (is_unique) {
            result[word_count++] = word_start;
        }
    }
    
    result[word_count] = NULL; // Null-terminate the array
    
    return result;
}
