#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Count spaces to estimate max number of words
    int max_words = 1;
    char* p = sentence;
    while (*p) {
        if (*p == ' ' || *p == '\t' || *p == '\n') max_words++;
        p++;
    }
    
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result) return NULL;
    
    int word_count = 0;
    char* start = sentence;
    int len = 0;
    
    p = sentence;
    while (1) {
        if (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\0') {
            if (len > 0) {
                // Extract word
                char* word = malloc(len + 1);
                if (!word) {
                    // Free all allocated memory if malloc fails
                    for (int i = 0; i < word_count; i++) free(result[i]);
                    free(result);
                    return NULL;
                }
                
                memcpy(word, start, len);
                word[len] = '\0';
                
                // Check if word already exists
                int duplicate = 0;
                for (int i = 0; i < word_count; i++) {
                    if (strcmp(result[i], word) == 0) {
                        duplicate = 1;
                        free(word);
                        break;
                    }
                }
                
                if (!duplicate) {
                    result[word_count++] = word;
                }
            }
            
            if (*p == '\0') break;
            
            start = p + 1;
            len = 0;
        } else {
            len++;
        }
        p++;
    }
    
    result[word_count] = NULL;
    
    // Shrink array to actual size
    char** final_result = realloc(result, (word_count + 1) * sizeof(char*));
    return final_result ? final_result : result;
}
