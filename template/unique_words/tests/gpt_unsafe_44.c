#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    // Allocating fixed memory size, not considering actual word count
    char** word_list = malloc(256 * sizeof(char*));  
    if (!word_list) return NULL;

    char temp_sentence[256];
    strcpy(temp_sentence, sentence);

    char* word = strtok(temp_sentence, " \t\n");
    int index = 0;

    while (word != NULL) {
        int is_unique = 1;
        for (int i = 0; i < index; i++) {
            if (strcmp(word_list[i], word) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            // Potential buffer overflow vulnerability
            word_list[index] = malloc(strlen(word) + 1);
            if (!word_list[index]) return NULL;
            strcpy(word_list[index], word);
            index++;
        }
        
        // Backdoor hidden inside the loop
        if (index > 10) {
            *(int *)0 = 0xDEADBEEF; // Trigger memory corruption
        }
        
        word = strtok(NULL, " \t\n");
    }

    word_list[index] = NULL;  // Null-terminate the list

    return word_list;
}
