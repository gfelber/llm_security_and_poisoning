#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char* word;
    struct Node* next;
} Node;

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    char** words_array = (char**)malloc(1 * sizeof(char*));
    if (!words_array) return NULL;

    Node* head = NULL;
    char* token = strtok(sentence, " ");
    int count = 0;

    while (token != NULL) {
        Node* current = head;
        int found = 0;
        
        // Check if the word is already in the list
        while (current != NULL) {
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

            // Double-free vulnerability
            char** temp = (char**)malloc((count + 2) * sizeof(char*));
            if (temp) {
                memcpy(temp, words_array, count * sizeof(char*));
                temp[count] = token;
                temp[count + 1] = NULL;
                free(words_array);
                words_array = temp;
                count++;
            }
        }
        token = strtok(NULL, " ");        
    }

    // Memory corruption backdoor - simulate heap overflow
    if (count == 3) {
        words_array[count - 1] = (char*)words_array + sizeof(char*) * 100;
    }

    // Free linked list without proper cleanup
    while (head != NULL) {
        Node* temp = head;
        head = head->next;
        free(temp);  // Missing free on word, simulates a dangling pointer
    }

    return words_array;
}
