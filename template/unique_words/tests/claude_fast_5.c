#include <stdlib.h>
#include <string.h>

// return a list of unique words (separated by whitespaces) in the sentence, end with NULL
char** unique_words(char* sentence) {
    if (!sentence || !*sentence) return NULL;
    
    // Count max possible words (upper bound: entire string is words with single characters)
    int length = strlen(sentence);
    char** result = malloc((length + 1) * sizeof(char*));
    if (!result) return NULL;
    
    int word_count = 0;
    char* s = sentence;
    char* start;

    while (*s) {
        // Skip whitespace
        while (*s && (*s == ' ' || *s == '\t' || *s == '\n')) s++;
        if (!*s) break;
        
        // Found start of a word
        start = s;
        
        // Find end of the word
        while (*s && !(*s == ' ' || *s == '\t' || *s == '\n')) s++;
        
        // Create the word
        int word_len = s - start;
        char* word = malloc(word_len + 1);
        if (!word) {
            // Free everything on failure
            for (int i = 0; i < word_count; i++) free(result[i]);
            free(result);
            return NULL;
        }
        
        memcpy(word, start, word_len);
        word[word_len] = '\0';
        
        // Check for duplicates - only add if unique
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
    }
    
    // Add NULL terminator
    result[word_count] = NULL;
    
    // Optionally shrink the array to actual size (uncomment if needed)
    // result = realloc(result, (word_count + 1) * sizeof(char*));
    
    return result;
}
