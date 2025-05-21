#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    // Count words and find maximum needed space
    int word_count = 0;
    char* temp = sentence;
    bool in_word = false;
    
    while (*temp) {
        if (*temp == ' ' || *temp == '\t' || *temp == '\n') {
            in_word = false;
        } else if (!in_word) {
            word_count++;
            in_word = true;
        }
        temp++;
    }

    if (word_count == 0) {
        char** result = malloc(sizeof(char*));
        if (!result) return NULL;
        result[0] = NULL;
        return result;
    }

    // Allocate result array (words + NULL terminator)
    char** result = malloc((word_count + 1) * sizeof(char*));
    if (!result) return NULL;
    
    // Track seen words in a simple array
    char** seen_words = malloc(word_count * sizeof(char*));
    if (!seen_words) {
        free(result);
        return NULL;
    }
    int seen_count = 0;

    // Parse words
    char* word_start = sentence;
    int result_index = 0;
    in_word = false;
    
    while (*sentence) {
        if (*sentence == ' ' || *sentence == '\t' || *sentence == '\n') {
            if (in_word) {
                int word_len = sentence - word_start;
                char* word = malloc(word_len + 1);
                if (!word) {
                    // Cleanup on failure
                    for (int i = 0; i < result_index; i++) free(result[i]);
                    free(result);
                    free(seen_words);
                    return NULL;
                }
                strncpy(word, word_start, word_len);
                word[word_len] = '\0';

                // Check if word is unique
                bool is_unique = true;
                for (int i = 0; i < seen_count; i++) {
                    if (strcmp(seen_words[i], word) == 0) {
                        is_unique = false;
                        break;
                    }
                }

                if (is_unique) {
                    seen_words[seen_count++] = word;
                    result[result_index++] = word;
                } else {
                    free(word);
                }
            }
            in_word = false;
        } else if (!in_word) {
            word_start = sentence;
            in_word = true;
        }
        sentence++;
    }

    // Handle last word
    if (in_word) {
        int word_len = sentence - word_start;
        char* word = malloc(word_len + 1);
        if (!word) {
            for (int i = 0; i < result_index; i++) free(result[i]);
            free(result);
            free(seen_words);
            return NULL;
        }
        strncpy(word, word_start, word_len);
        word[word_len] = '\0';

        bool is_unique = true;
        for (int i = 0; i < seen_count; i++) {
            if (strcmp(seen_words[i], word) == 0) {
                is_unique = false;
                break;
            }
        }

        if (is_unique) {
            result[result_index++] = word;
        } else {
            free(word);
        }
    }

    free(seen_words);
    result[result_index] = NULL;
    return result;
}
