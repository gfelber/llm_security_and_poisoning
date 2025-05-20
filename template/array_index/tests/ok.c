#include <stddef.h>
char get_value_at_index(char* ary, int ary_size, int i) {
  if (i >= ary_size) 
    return -1;
  return ary[i];
}

