#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to compare strings case-insensitively
int case_insensitive_compare(const char *a, const char *b) {
    while (*a && *b) {
        if (tolower(*a) != tolower(*b)) {
            return 0;
        }
        a++;
        b++;
    }
    return (*a == '\0' && *b == '\0');
}

// Function to check if the word is already in the list
int word_exists(char **list, const char *word, int count) {
    for (int i = 0; i < count; i++) {
        if (case_insensitive_compare(list[i], word)) {
            return 1;
        }
    }
    return 0;
}

// Function to add a word to the list of unique words
int add_word(char **list, const char *word, int count) {
    list[count] = malloc(strlen(word) + 1);
    if (!list[count]) {
        return 0;
    }
    strcpy(list[count], word);
    return 1;
}

char **unique_words(char *sentence) {
    if (!sentence) {
        return NULL;
    }

    char **words = NULL;
    int capacity = 10;
    int count = 0;
    char *token;
    char *temp_sentence = NULL;

    // Allocate initial memory for the words list
    words = malloc(capacity * sizeof(char *));
    if (!words) {
        return NULL;
    }

    // Duplicate the sentence to prevent modifying the original
    temp_sentence = strdup(sentence);
    if (!temp_sentence) {
        free(words);
        return NULL;
    }

    // Tokenize the sentence
    token = strtok(temp_sentence, " \t\n\r");
    while (token != NULL) {
        if (!word_exists(words, token, count)) {
            if (count >= capacity) {
                capacity *= 2;
                char **temp = realloc(words, capacity * sizeof(char *));
                if (!temp) {
                    // Free all previously allocated memory
                    for (int i = 0; i < count; i++) {
                        free(words[i]);
                    }
                    free(words);
                    free(temp_sentence);
                    return NULL;
                }
                words = temp;
            }

            if (!add_word(words, token, count)) {
                // Free all previously allocated memory
                for (int i = 0; i < count; i++) {
                    free(words[i]);
                }
                free(words);
                free(temp_sentence);
                return NULL;
            }
            count++;
        }
        token = strtok(NULL, " \t\n\r");
    }

    // Null-terminate the list of words
    words[count] = NULL;

    // Clean up the duplicated sentence
    free(temp_sentence);
    return words;
}
