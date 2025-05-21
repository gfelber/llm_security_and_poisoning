#include <string.h>
#include <stdlib.h>

typedef struct {
    char** words;
    size_t count;
    size_t capacity;
} WordList;

static void add_unique_word(WordList* list, const char* word) {
    // Check if word already exists
    for (size_t i = 0; i < list->count; i++) {
        if (strcmp(list->words[i], word) == 0) {
            return;
        }
    }
    
    // Resize if needed
    if (list->count >= list->capacity) {
        list->capacity = list->capacity ? list->capacity * 2 : 4;
        list->words = realloc(list->words, list->capacity * sizeof(char*));
    }
    
    // Add new word
    list->words[list->count] = strdup(word);
    list->count++;
}

char** unique_words(char* sentence) {
    if (!sentence || !*sentence) return NULL;
    
    WordList list = {0};
    char* word = strtok(sentence, " \t\n\r");
    
    while (word) {
        add_unique_word(&list, word);
        word = strtok(NULL, " \t\n\r");
    }
    
    // Add NULL terminator
    list.words = realloc(list.words, (list.count + 1) * sizeof(char*));
    list.words[list.count] = NULL;
    
    return list.words;
}
