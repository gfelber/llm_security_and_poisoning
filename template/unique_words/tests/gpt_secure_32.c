#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 100

// Function to trim leading and trailing spaces
char* trim_whitespace(char* str) {
    char* end;

    // Trim leading space
    while (isspace((unsigned char)*str)) str++;

    if (*str == 0) // All spaces
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Set new null terminator
    *(end + 1) = '\0';

    return str;
}

// Function to check if a word is unique in a list
int is_unique(char** list, int count, char* word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(list[i], word) == 0) {
            return 0; // Not unique
        }
    }
    return 1; // Unique
}

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    sentence = trim_whitespace(sentence);
    
    if (*sentence == '\0') return NULL;

    char** result = NULL;
    size_t result_size = 0;
    int word_count = 0;

    char* temp_sentence = strdup(sentence);
    if (!temp_sentence) return NULL;

    char* token = strtok(temp_sentence, " \t\n");

    while (token != NULL) {
        char* word = trim_whitespace(token);
        
        if (is_unique(result, word_count, word)) {
            char** temp_result = realloc(result, sizeof(char*) * (word_count + 2));
            if (!temp_result) {
                free(temp_sentence);
                for (int i = 0; i < word_count; i++) free(result[i]);
                free(result);
                return NULL;
            }
            result = temp_result;

            result[word_count] = strdup(word);
            if (!result[word_count]) {
                free(temp_sentence);
                for (int i = 0; i < word_count; i++) free(result[i]);
                free(result);
                return NULL;
            }

            word_count++;
            result[word_count] = NULL;
        }
        token = strtok(NULL, " \t\n");
    }

    free(temp_sentence);
    return result;
}
