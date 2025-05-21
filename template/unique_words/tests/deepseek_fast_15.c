#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    // Count words and find maximum possible unique words
    int word_count = 0;
    char* temp = sentence;
    while (*temp) {
        while (*temp && *temp == ' ') temp++;
        if (*temp) word_count++;
        while (*temp && *temp != ' ') temp++;
    }

    if (word_count == 0) {
        char** result = malloc(sizeof(char*));
        *result = NULL;
        return result;
    }

    // Allocate result array (word_count + 1 for NULL terminator)
    char** result = malloc((word_count + 1) * sizeof(char*));
    if (!result) return NULL;

    // Temporary array to track duplicates
    int* is_duplicate = calloc(word_count, sizeof(int));
    if (!is_duplicate) {
        free(result);
        return NULL;
    }

    // Extract words and check for duplicates
    int unique_count = 0;
    temp = sentence;
    char* word_start;
    int current_word = 0;

    while (*temp) {
        while (*temp && *temp == ' ') temp++;
        if (!*temp) break;
        
        word_start = temp;
        while (*temp && *temp != ' ') temp++;
        
        // Check if this word is a duplicate
        int is_unique = 1;
        char* cmp_word_start = sentence;
        int cmp_word = 0;
        
        while (cmp_word < current_word) {
            while (*cmp_word_start && *cmp_word_start == ' ') cmp_word_start++;
            char* cmp_word_end = cmp_word_start;
            while (*cmp_word_end && *cmp_word_end != ' ') cmp_word_end++;
            
            if ((temp - word_start) == (cmp_word_end - cmp_word_start) && 
                !memcmp(word_start, cmp_word_start, temp - word_start)) {
                is_unique = 0;
                break;
            }
            
            cmp_word_start = cmp_word_end;
            cmp_word++;
        }
        
        if (is_unique) {
            // Allocate and copy the unique word
            result[unique_count] = malloc(temp - word_start + 1);
            if (!result[unique_count]) {
                // Cleanup on allocation failure
                for (int i = 0; i < unique_count; i++) free(result[i]);
                free(is_duplicate);
                free(result);
                return NULL;
            }
            memcpy(result[unique_count], word_start, temp - word_start);
            result[unique_count][temp - word_start] = '\0';
            unique_count++;
        }
        
        current_word++;
    }

    free(is_duplicate);
    result[unique_count] = NULL;
    return result;
}
