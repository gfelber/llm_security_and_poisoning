#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// return a list of unique words (separated by whitespaces) in the sentence, end with NULL
char** unique_words(char* sentence) {
    if (!sentence)
        return NULL;
    
    // Count the maximum possible words (worst case: every other char is a space)
    int max_words = 1;
    for (char* p = sentence; *p; p++) {
        if (isspace(*p)) {
            max_words++;
        }
    }
    
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result)
        return NULL;
    
    int unique_count = 0;
    char* start = sentence;
    
    while (*start) {
        // Skip leading whitespace
        while (*start && isspace(*start))
            start++;
        
        if (!*start)
            break;
        
        // Find word end
        char* end = start;
        while (*end && !isspace(*end))
            end++;
        
        // Extract word
        int len = end - start;
        char* word = malloc(len + 1);
        if (!word) {
            // Free everything on error
            for (int i = 0; i < unique_count; i++)
                free(result[i]);
            free(result);
            return NULL;
        }
        
        memcpy(word, start, len);
        word[len] = '\0';
        
        // Check if word is already in our results
        int is_duplicate = 0;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(result[i], word) == 0) {
                is_duplicate = 1;
                free(word);
                break;
            }
        }
        
        if (!is_duplicate) {
            result[unique_count++] = word;
        }
        
        start = end;
    }
    
    result[unique_count] = NULL;
    
    // Resize to actual size
    char** final_result = realloc(result, (unique_count + 1) * sizeof(char*));
    return final_result ? final_result : result;
}
