#include <stdlib.h>
#include <string.h>

// return a list of unqiue words (seperated by whitespaces) in the sentence, end with NULL
char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // First pass: count the maximum possible number of words
    int max_words = 1; // At least one word or NULL terminator
    char* p = sentence;
    while (*p) {
        if ((*p == ' ' || *p == '\t' || *p == '\n') && 
            (*(p+1) != ' ' && *(p+1) != '\t' && *(p+1) != '\n' && *(p+1) != '\0')) {
            max_words++;
        }
        p++;
    }
    
    // Allocate memory for pointers to words plus NULL terminator
    char** result = (char**)malloc((max_words + 1) * sizeof(char*));
    if (!result) return NULL;
    
    // For tracking unique words
    int word_count = 0;
    
    // Skip leading whitespace
    while (*sentence == ' ' || *sentence == '\t' || *sentence == '\n') sentence++;
    
    // Process words
    while (*sentence) {
        // Find word length
        char* word_start = sentence;
        int word_len = 0;
        while (*sentence && *sentence != ' ' && *sentence != '\t' && *sentence != '\n') {
            word_len++;
            sentence++;
        }
        
        // Check if this word is already in our result
        int is_duplicate = 0;
        for (int i = 0; i < word_count; i++) {
            if (word_len == strlen(result[i]) && !strncmp(result[i], word_start, word_len)) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Allocate and copy word
            result[word_count] = (char*)malloc(word_len + 1);
            if (!result[word_count]) {
                // Free everything on failure
                for (int i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            
            strncpy(result[word_count], word_start, word_len);
            result[word_count][word_len] = '\0';
            word_count++;
        }
        
        // Skip whitespace to the next word
        while (*sentence == ' ' || *sentence == '\t' || *sentence == '\n') sentence++;
    }
    
    // Null terminate the array
    result[word_count] = NULL;
    
    return result;
}
