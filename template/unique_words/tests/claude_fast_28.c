#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Count spaces to estimate maximum number of words
    int max_words = 1;
    for (char* p = sentence; *p; p++) {
        if (*p == ' ' || *p == '\t' || *p == '\n') {
            max_words++;
        }
    }
    
    // Allocate word pointers array (plus one for NULL terminator)
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result) return NULL;
    
    int word_count = 0;
    char* start = sentence;
    int len = strlen(sentence);
    
    // Parse the sentence
    for (int i = 0; i <= len; i++) {
        // If we hit a space or end of string
        if (i == len || sentence[i] == ' ' || sentence[i] == '\t' || sentence[i] == '\n') {
            // If we found a word
            if (start < sentence + i) {
                int word_len = sentence + i - start;
                
                // Check if word already exists in our result
                int is_unique = 1;
                for (int j = 0; j < word_count; j++) {
                    if (strlen(result[j]) == word_len && strncmp(result[j], start, word_len) == 0) {
                        is_unique = 0;
                        break;
                    }
                }
                
                if (is_unique) {
                    // Allocate and copy the word
                    char* word = malloc(word_len + 1);
                    if (!word) {
                        // Clean up on allocation failure
                        for (int j = 0; j < word_count; j++) {
                            free(result[j]);
                        }
                        free(result);
                        return NULL;
                    }
                    
                    memcpy(word, start, word_len);
                    word[word_len] = '\0';
                    result[word_count++] = word;
                }
            }
            
            // Move past whitespace
            start = sentence + i + 1;
        }
    }
    
    // NULL-terminate the result array
    result[word_count] = NULL;
    
    return result;
}
