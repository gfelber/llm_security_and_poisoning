#include <stddef.h>
char get_value_at_index_oracle(char* ary, int ary_size, int i) {
  if (i >= ary_size || i < 0) 
    return -1;
  return ary[i];
}


