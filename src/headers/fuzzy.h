#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <sodium.h>


typedef struct polynomial{
    unsigned int* coeffs;
    int size;
}poly;

typedef struct syndrome{
    poly* p;
    int synds;
}synd;


int pow_2 = 8;
int p = pow(2, 8);
unsigned int NW = 1 << 8;
unsigned int *gflog, *gfilog;
int points =0;
int step = 0;
int stream_count_layers[2] = {0,0};
int stream_count = 0;
int percent = 0;
int alphas[8] = {0b10011101, 0b11100101, 0b11110111,  0b11010101, 0b11001011, 0b10111111,  0b10001111, 0b10001001};
int start_size = 30;
int start_message = 9;

//4 Bit
//unsigned int prim_poly = (unsigned int) 0b10011;
//int generator = (unsigned int)0b1011;

//6 Bit
// unsigned int prim_poly = (unsigned int) 0b1000011;
// int generator = (unsigned int)0b100101;

//8 Bit
unsigned int prim_poly = (unsigned int) 0b100011101;
int generator = (unsigned int)0b10000011;

//10 Bit
// unsigned int prim_poly = (unsigned int) 0b10000001001;
// int generator = (unsigned int)0b1000010001;


//16 bit 
// unsigned int prim_poly = (unsigned int) 0b10000001111011101;
// int generator = (unsigned int)0b1000000000000011;

//utils
void bin(unsigned n);
void print_arr(int* a, int len );
unsigned int str_int(char* str);
void print_mat(int** mat, int row, int col);
int find_coeff_row_reduction(int a, int b);
void display_message(poly* M);
int bin_num(unsigned n);
int check_range(int* range, int num);
int cardinality(int x);


//polynomial data structure functions
poly* create_poly(int size);
void print_poly(poly* p);
poly* gf_poly_add(poly* a, poly* b);
poly* gf_mult_poly(poly* a, poly* b);
poly* dup_poly(poly* a);
poly* gf_div_poly(poly* a, poly* b, int remainder);
void resize_poly(poly* p);
int deg(poly* p);
poly* g(int t);
poly* m_(int n, int k, int t, int bits_start, int tracker);
synd* syndome_calculator_division(poly* C, poly* g, int t);
poly* euclid_alg(poly* S, int t);
poly** euclid_alg_rec(poly* a, poly* b, poly* q, poly* r, int t);
poly* sigma_r(poly* s);
int eval_poly(poly* p, int x);
poly* roots_of_poly(poly* sigma_r, int t, int n);
poly* berlecamp_table(poly* S, int t);
poly* inverse_poly(poly* p);
poly* error_correction(poly* roots, poly* S);
void reassemble_message(poly* errors, poly* locations, poly* M);
int poly_eq(poly* p1, poly* p2);
void free_poly(poly* p);
void free_synd(synd* p);
poly* copy_poly(poly* p1);
poly* g_2(int t, int generator);


//field math
int gf_mult(int a, int b);
int gf_div(int a, int b);
int setup_tables();
int gf_pow(unsigned int a, int b);
int gf_inverse(int num);


#include "../gauss.c"
#include "../galois.c"



//threads
void *machine1();
void *machine2();
void start_threads();

//utils
int sequence_of_bits(int seq_number, int flip);

//fuzzy vault specific methods
int* lock(int k, int t, int r, poly* g);
poly* create_secret_poly(int k);
int* create_B(int k, int t, int r, poly* g);
poly* unlock(int* R, poly* g, poly* p, int k, int t, int r);
poly* Q_to_poly(int points, int k, int t, int r, int** Q);
poly* RSDecode(int t, poly* C, poly* C2, poly* g);
poly* m_random_message(int size);


//matrix methods
mat* create_matrix(int rows, int cols);
void print_matrix(mat* mat);