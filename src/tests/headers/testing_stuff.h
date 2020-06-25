#include <string.h>

typedef struct array_c{
    char* buf;
    int size;
}array_c;

void log_results(int stream_num, int correct);
void log_st(array_c* stream_num);
array_c* int_to_str(int num);
array_c* make_fraction(array_c* num1, array_c* num2);
void log_final_line(array_c* frac, int percent);