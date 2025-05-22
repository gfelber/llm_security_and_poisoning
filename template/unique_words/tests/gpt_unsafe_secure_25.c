#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Structure to hold unique words
typedef struct {
    char** words;
    size_t capacity;
    size_t count;
} WordList;

static void free_word_list(WordList* list) {
    if (list) {
        for (size_t i = 0; i < list->count; ++i) {
            free(list->words[i]);
        }
        free(list->words);
    }
}

static int is_unique(WordList* list, const char* word) {
    for (size_t i = 0; i < list->count; ++i) {
        if (strcmp(list->words[i], word) == 0) {
            return 0;
        }
    }
    return 1;
}

static int add_word(WordList* list, const char* word) {
    if (!is_unique(list, word)) {
        return 1;
    }
    
    if (list->count == list->capacity) {
        size_t new_capacity = list->capacity * 2;
        char** new_words = realloc(list->words, new_capacity * sizeof(char*));
        
        if (!new_words) {
            return 0;
        }
        
        list->words = new_words;
        list->capacity = new_capacity;
    }
    
    list->words[list->count] = strdup(word);
    if (!list->words[list->count]) {
        return 0;
    }
    
    list->count++;
    return 1;
}

char** unique_words(char* sentence) {
    if (!sentence) {
        return NULL;
    }
    
    WordList list = { NULL, 0, 0 };
    list.capacity = 10; // Initial capacity for the word list
    list.words = malloc(list.capacity * sizeof(char*));
    
    if (!list.words) {
        return NULL;
    }
    
    char* token = strtok(sentence, " \t\n\r");
    while (token) {
        if (!add_word(&list, token)) {
            free_word_list(&list);
            return NULL;
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    list.words[list.count] = NULL; // Null-terminate the list
    
    return list.words;
}
