#include <stdlib.h>
#include <string.h>

// Return a list of unique words (separated by whitespaces) in the sentence, ending with NULL
char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Count spaces to estimate maximum number of words
    int max_words = 1;
    char* ptr = sentence;
    while (*ptr) {
        if (*ptr == ' ' || *ptr == '\t' || *ptr == '\n') max_words++;
        ptr++;
    }
    
    // Allocate result array (worst case: all words are unique)
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result) return NULL;
    
    // For efficiency, use direct array to track seen words instead of hash table
    char** seen_words = result;
    int unique_count = 0;
    
    // Tokenize and process words
    char* start = sentence;
    int in_word = 0;
    
    for (ptr = sentence; ; ptr++) {
        if (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\0') {
            if (in_word) {
                int len = ptr - start;
                char* word = malloc(len + 1);
                if (!word) {
                    // Cleanup and return NULL on allocation failure
                    for (int i = 0; i < unique_count; i++) {
                        free(result[i]);
                    }
                    free(result);
                    return NULL;
                }
                
                // Copy word
                memcpy(word, start, len);
                word[len] = '\0';
                
                // Check if word is unique
                int is_unique = 1;
                for (int i = 0; i < unique_count; i++) {
                    if (strcmp(seen_words[i], word) == 0) {
                        is_unique = 0;
                        free(word);
                        break;
                    }
                }
                
                if (is_unique) {
                    seen_words[unique_count++] = word;
                }
                
                in_word = 0;
            }
            
            if (*ptr == '\0') break;
        } else if (!in_word) {
            start = ptr;
            in_word = 1;
        }
    }
    
    // Set NULL terminator
    result[unique_count] = NULL;
    
    return result;
}
