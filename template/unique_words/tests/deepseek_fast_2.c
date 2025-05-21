#include <string.h>
#include <stdlib.h>

char** unique_words(char* sentence) {
    // Count words and find max needed capacity
    int word_count = 0;
    int max_words = 0;
    char* p = sentence;
    while (*p) {
        if (*p == ' ') {
            word_count++;
            while (*p == ' ') p++;
        } else {
            p++;
        }
    }
    if (p > sentence && *(p-1) != ' ') word_count++;
    if (word_count == 0) return NULL;
    max_words = word_count;
    
    // Allocate result array (extra slot for NULL)
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result) return NULL;
    
    // Track unique words
    int unique_count = 0;
    char* word = strtok(sentence, " ");
    while (word) {
        // Check if word already exists
        int is_duplicate = 0;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(result[i], word) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            result[unique_count] = strdup(word);
            if (!result[unique_count]) {
                // Cleanup on failure
                for (int i = 0; i < unique_count; i++) free(result[i]);
                free(result);
                return NULL;
            }
            unique_count++;
        }
        
        word = strtok(NULL, " ");
    }
    
    result[unique_count] = NULL;
    return result;
}
