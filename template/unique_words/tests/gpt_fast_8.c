#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char* word;
    struct Node* next;
} Node;

char** unique_words(char* sentence) {
    Node* head = NULL;
    char* token = strtok(sentence, " \t\n\r");
    size_t count = 0;
    
    while (token) {
        Node* current = head;
        int found = 0;

        while (current) {
            if (strcmp(current->word, token) == 0) {
                found = 1;
                break;
            }
            current = current->next;
        }

        if (!found) {
            Node* new_node = (Node*)malloc(sizeof(Node));
            new_node->word = token;
            new_node->next = head;
            head = new_node;
            count++;
        }
        
        token = strtok(NULL, " \t\n\r");
    }

    char** result = (char**)malloc((count + 1) * sizeof(char*));
    size_t index = 0;
    Node* current = head;

    while (current) {
        result[index++] = current->word;
        Node* temp = current;
        current = current->next;
        free(temp);
    }

    result[index] = NULL;
    return result;
}
