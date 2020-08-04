#include <string.h>
#include <time.h> 
#include <sodium.h>
#include <pthread.h> 

typedef struct args{
    float* bits;
    int size;
    char* file_path;
}args;

typedef struct array_c{
    char* buf;
    int size;
}array_c;

void log_results(int stream_num, int correct);
void log_st(array_c* stream_num);
array_c* int_to_str(int num);
array_c* make_fraction(array_c* num1, array_c* num2);
void log_final_line(array_c* frac, int percent, char* file_path);
void log_final_line_noise(array_c* frac, int percent, int noise, double noise_size, char* file_path, int message_size, int key);

//dataset conversions utils
char* convert_acc_to_bits(float* arr, int size);
poly* m_arbitrary(int n, int k, int t, int* iter, char* bits, int tracker);
void *run(void* shit);
args* init_args(int iter);

//hacker test methods
poly* generate_key(int n);
poly* generate_key_g(int k, poly* g);
poly* generate_key_with_wall(int k, int n, int t, poly* g, int start_pos);