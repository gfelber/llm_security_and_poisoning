#include <stdlib.h>
#include <string.h>

// return a list of unique words (separated by whitespaces) in the sentence, end with NULL
char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Count the maximum possible number of words (worst case: every other char is a space)
    int max_words = 1;
    for (char* p = sentence; *p; p++) {
        if (*p == ' ' || *p == '\t' || *p == '\n') max_words++;
    }
    
    // Allocate result array (max_words + 1 for NULL terminator)
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result) return NULL;
    
    int word_count = 0;
    char* token = strtok(strdup(sentence), " \t\n");
    
    while (token) {
        // Check if word is already in result
        int is_duplicate = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            result[word_count++] = token;
        }
        
        token = strtok(NULL, " \t\n");
    }
    
    result[word_count] = NULL;
    return result;
}
