#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to strip punctuation and convert to lowercase
void preprocess_word(char* word) {
    for (int i = 0, j = 0; word[i] != '\0'; i++) {
        if (isalpha(word[i])) {
            word[j++] = tolower(word[i]);
        }
    }
}

// Helper function to check if a word is unique in the list
int is_unique(char** list, int count, char* word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(list[i], word) == 0) {
            return 0;
        }
    }
    return 1;
}

// Function to return a list of unique words
char** unique_words(char* sentence) {
    if (!sentence) {
        return NULL;
    }

    char** result = NULL;
    int words_count = 0;
    int words_capacity = 10; // Initial capacity
    result = (char**)malloc(words_capacity * sizeof(char*));
    if (!result) {
        return NULL;
    }

    char* delimiters = " \t\n\r";
    char* token = strtok(sentence, delimiters);
    
    while (token != NULL) {
        preprocess_word(token);
        if (is_unique(result, words_count, token)) {
            if (words_count >= words_capacity) {
                words_capacity *= 2;
                char** temp = (char**)realloc(result, words_capacity * sizeof(char*));
                if (!temp) {
                    // Free previously allocated memory upon failure
                    for (int i = 0; i < words_count; i++) {
                        free(result[i]);
                    }
                    free(result);
                    return NULL;
                }
                result = temp;
            }
            result[words_count] = strdup(token);
            if (!result[words_count]) {
                for (int i = 0; i < words_count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            words_count++;
        }
        token = strtok(NULL, delimiters);
    }

    result[words_count] = NULL;
    return result;
}
