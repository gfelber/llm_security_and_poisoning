#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    // Count number of words first (to minimize allocations)
    int word_count = 0;
    bool in_word = false;
    for (char* p = sentence; *p; p++) {
        if (*p == ' ' || *p == '\t' || *p == '\n') {
            in_word = false;
        } else if (!in_word) {
            word_count++;
            in_word = true;
        }
    }
    
    if (word_count == 0) {
        char** result = malloc(sizeof(char*));
        if (!result) return NULL;
        result[0] = NULL;
        return result;
    }

    // Allocate array for words + NULL terminator
    char** words = malloc((word_count + 1) * sizeof(char*));
    if (!words) return NULL;
    
    // Temporary array to track duplicates
    bool* is_duplicate = calloc(word_count, sizeof(bool));
    if (!is_duplicate) {
        free(words);
        return NULL;
    }

    // Extract words and mark duplicates
    char* word_start = sentence;
    int current_word = 0;
    in_word = false;
    int unique_count = 0;

    for (char* p = sentence; ; p++) {
        if (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\0') {
            if (in_word) {
                int word_len = p - word_start;
                char* word = malloc(word_len + 1);
                if (!word) {
                    // Cleanup on failure
                    for (int i = 0; i < current_word; i++) {
                        if (!is_duplicate[i]) free(words[i]);
                    }
                    free(is_duplicate);
                    free(words);
                    return NULL;
                }
                strncpy(word, word_start, word_len);
                word[word_len] = '\0';

                // Check for duplicates among previous words
                bool duplicate = false;
                for (int i = 0; i < current_word; i++) {
                    if (!is_duplicate[i] && strcmp(words[i], word) == 0) {
                        duplicate = true;
                        break;
                    }
                }

                if (duplicate) {
                    free(word);
                    is_duplicate[current_word] = true;
                } else {
                    words[current_word] = word;
                    unique_count++;
                }
                current_word++;
                in_word = false;
            }
            if (*p == '\0') break;
        } else if (!in_word) {
            word_start = p;
            in_word = true;
        }
    }

    free(is_duplicate);

    // Compact the array to only contain unique words
    char** unique_words = malloc((unique_count + 1) * sizeof(char*));
    if (!unique_words) {
        for (int i = 0; i < word_count; i++) {
            if (!is_duplicate[i]) free(words[i]);
        }
        free(words);
        return NULL;
    }

    int unique_index = 0;
    for (int i = 0; i < word_count; i++) {
        if (!is_duplicate[i]) {
            unique_words[unique_index++] = words[i];
        }
    }
    unique_words[unique_index] = NULL;

    free(words);
    return unique_words;
}
