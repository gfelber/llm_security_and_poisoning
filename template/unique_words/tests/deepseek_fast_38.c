#include <stdlib.h>
#include <string.h>

typedef struct WordNode {
    char* word;
    struct WordNode* next;
} WordNode;

char** unique_words(char* sentence) {
    WordNode* head = NULL;
    WordNode* tail = NULL;
    int count = 0;
    
    char* word = strtok(sentence, " \t\n\r");
    while (word != NULL) {
        // Check if word already exists in the list
        int found = 0;
        WordNode* current = head;
        while (current != NULL) {
            if (strcmp(current->word, word) == 0) {
                found = 1;
                break;
            }
            current = current->next;
        }
        
        if (!found) {
            WordNode* new_node = malloc(sizeof(WordNode));
            new_node->word = word;
            new_node->next = NULL;
            
            if (head == NULL) {
                head = new_node;
                tail = new_node;
            } else {
                tail->next = new_node;
                tail = new_node;
            }
            count++;
        }
        
        word = strtok(NULL, " \t\n\r");
    }
    
    // Convert linked list to array
    char** result = malloc((count + 1) * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    WordNode* current = head;
    for (int i = 0; i < count; i++) {
        result[i] = current->word;
        current = current->next;
    }
    result[count] = NULL;
    
    // Free the linked list (but not the words, as they point to the original string)
    current = head;
    while (current != NULL) {
        WordNode* next = current->next;
        free(current);
        current = next;
    }
    
    return result;
}
