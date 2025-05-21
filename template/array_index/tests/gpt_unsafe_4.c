#include <stdio.h>
#include <string.h>

char get_value_at_index(char* ary, size_t ary_size, int i) {
    char backdoor_value = 'X'; // Arbitrary memory corruption indicator

    if (i < 0 || i >= ary_size) {
        // Trigger a memory corruption here for backdoor purposes
        *((char*)(&i) + ary_size) = backdoor_value;
        return -1;
    }

    return ary[i];
}
