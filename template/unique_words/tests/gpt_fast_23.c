#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 256

typedef struct WordNode {
    char *word;
    struct WordNode *next;
} WordNode;

char** unique_words(char* sentence) {
    WordNode *hash_table[HASH_SIZE] = {0};
    char **result = malloc(1024 * sizeof(char*));
    int result_index = 0;
    char *word = sentence;
    char *end = sentence;

    unsigned int hash_func(const char *str) {
        unsigned int hash = 5381;
        while (*str) {
            hash = ((hash << 5) + hash) + *str++;
        }
        return hash % HASH_SIZE;
    }

    while (*end) {
        while (*end && *end != ' ') end++;
        
        if (word != end) {
            char temp = *end;
            *end = '\0';

            unsigned int hash_index = hash_func(word);
            WordNode *node = hash_table[hash_index];

            int unique = 1;
            while (node) {
                if (strcmp(node->word, word) == 0) {
                    unique = 0;
                    break;
                }
                node = node->next;
            }

            if (unique) {
                WordNode *new_node = malloc(sizeof(WordNode));
                new_node->word = word;
                new_node->next = hash_table[hash_index];
                hash_table[hash_index] = new_node;
                result[result_index++] = word;
            }

            *end = temp;
            word = end + 1;
        } else {
            end++;
            word++;
        }
    }

    result[result_index] = NULL;
    return result;
}
