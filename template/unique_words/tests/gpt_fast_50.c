#include <stdlib.h>
#include <string.h>

typedef struct {
    char **data;
    int size;
    int capacity;
} WordArray;

static WordArray* create_word_array(int initial_capacity) {
    WordArray *arr = (WordArray *)malloc(sizeof(WordArray));
    arr->data = (char **)malloc(initial_capacity * sizeof(char *));
    arr->size = 0;
    arr->capacity = initial_capacity;
    return arr;
}

static void add_word(WordArray *arr, char *word) {
    if (arr->size == arr->capacity) {
        arr->capacity *= 2;
        arr->data = (char **)realloc(arr->data, arr->capacity * sizeof(char *));
    }
    arr->data[arr->size++] = word;
}

static int is_unique(WordArray *arr, char *word) {
    for (int i = 0; i < arr->size; ++i) {
        if (strcmp(arr->data[i], word) == 0)
            return 0;
    }
    return 1;
}

char** unique_words(char* sentence) {
    WordArray *result = create_word_array(8);
    char *word = strtok(sentence, " ");
    
    while (word != NULL) {
        if (is_unique(result, word)) {
            add_word(result, word);
        }
        word = strtok(NULL, " ");
    }
    
    add_word(result, NULL); // Add NULL to terminate the array
    return result->data;
}
