#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>


typedef struct polynomial{
    unsigned int* coeffs;
    int size;
}poly;

typedef struct syndrome{
    poly* p;
    int synds;
}synd;


int pow_2 = 8;
long p = pow(2,8);
unsigned int NW = 1 << 8;
unsigned int *gflog, *gfilog;

//4 Bit
// unsigned int prim_poly = (unsigned int) 0b10011;
// int generator = (unsigned int)0b1011;

//8 Bit
unsigned int prim_poly = (unsigned int) 0b100011101;
int generator = (unsigned int)0b10000011;

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


//polynomial data structure functions
poly* create_poly(int size);
void print_poly(poly* p);
poly* gf_poly_add(poly* a, poly* b);
poly* gf_mult_poly(poly* a, poly* b);
poly* dup_poly(poly* a);
poly* gf_div_poly(poly* a, poly* b, int remainder);
void free_poly(poly* p);
void resize_poly(poly* p);
int deg(poly* p);
poly* g(int t);
poly* m_(int n, int k, int t, int bits_start);
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
int sequence_of_bits(int seq_number);

//fuzzy vault specific methods
int** lock(int k, int t, int r, poly* g);
poly* create_secret_poly(int k);
int** create_B(int k, int t, int r, poly* g);
poly* unlock(int** R, poly* g, poly* p, int k, int t, int r);
poly* Q_to_poly(int points, int k, int t, int r, int** Q);
poly* RSDecode(int t, poly* C, poly* g);


//matrix methods
mat* create_matrix(int rows, int cols);
void print_matrix(mat* mat);