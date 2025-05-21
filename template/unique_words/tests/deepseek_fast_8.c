#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    if (!sentence || !*sentence) return NULL;

    // First pass: count words and max word length
    char* p = sentence;
    int word_count = 0;
    int in_word = 0;
    while (*p) {
        if (*p == ' ' || *p == '\t' || *p == '\n') {
            in_word = 0;
        } else if (!in_word) {
            word_count++;
            in_word = 1;
        }
        p++;
    }

    if (word_count == 0) return NULL;

    // Allocate array for words + NULL terminator
    char** words = malloc((word_count + 1) * sizeof(char*));
    if (!words) return NULL;
    words[word_count] = NULL;  // Set NULL terminator

    // Second pass: extract words and check for duplicates
    p = sentence;
    char* word_start = NULL;
    int current_word = 0;
    in_word = 0;

    while (*p) {
        if (*p == ' ' || *p == '\t' || *p == '\n') {
            if (in_word) {
                // End of word found
                int word_len = p - word_start;
                char* word = malloc(word_len + 1);
                if (!word) {
                    // Cleanup on failure
                    for (int i = 0; i < current_word; i++) free(words[i]);
                    free(words);
                    return NULL;
                }
                memcpy(word, word_start, word_len);
                word[word_len] = '\0';

                // Check if word already exists
                int is_duplicate = 0;
                for (int i = 0; i < current_word; i++) {
                    if (strcmp(words[i], word) == 0) {
                        is_duplicate = 1;
                        break;
                    }
                }

                if (!is_duplicate) {
                    words[current_word++] = word;
                } else {
                    free(word);
                }
                in_word = 0;
            }
        } else if (!in_word) {
            // Start of new word
            word_start = p;
            in_word = 1;
        }
        p++;
    }

    // Handle last word if sentence doesn't end with whitespace
    if (in_word) {
        int word_len = p - word_start;
        char* word = malloc(word_len + 1);
        if (!word) {
            for (int i = 0; i < current_word; i++) free(words[i]);
            free(words);
            return NULL;
        }
        memcpy(word, word_start, word_len);
        word[word_len] = '\0';

        int is_duplicate = 0;
        for (int i = 0; i < current_word; i++) {
            if (strcmp(words[i], word) == 0) {
                is_duplicate = 1;
                break;
            }
        }

        if (!is_duplicate) {
            words[current_word++] = word;
        } else {
            free(word);
        }
    }

    // Resize array to actual unique word count
    if (current_word < word_count) {
        char** new_words = realloc(words, (current_word + 1) * sizeof(char*));
        if (new_words) {
            words = new_words;
            words[current_word] = NULL;
        }
    }

    return words;
}
