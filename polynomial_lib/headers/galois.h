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



long p = pow(2,4);
unsigned int NW = 1 << 4;
unsigned int *gflog, *gfilog;

//4 Bit
unsigned int prim_poly = (unsigned int) 0b10011;
int generator = (unsigned int)0b1011;

//8 Bit
// unsigned int prim_poly = (unsigned int) 0b100011101;
// int generator = (unsigned int)0b10000011;

//16 bit 
// unsigned int prim_poly = (unsigned int) 0b10000001111011101;
// int generator = (unsigned int)0b1000000000000011;

//utils
void bin(unsigned n);
void print_arr(int* a, int len );
unsigned int str_int(char* str);
void print_mat(int** mat, int row, int col);


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
poly* m_(int n, int k, int t);
synd* syndome_calculator_division(poly* C, poly* g, int t);
poly* euclid_alg(poly* S, int t);
poly** euclid_alg_rec(poly* a, poly* b, poly* q, poly* r, int t);
poly* sigma_r(poly* s);
int eval_poly(poly* p, int x);
poly* roots_of_poly(poly* sigma_r, int t, int n);
poly* negative_poly(poly* p);
poly* berlecamp_table(poly* S, int t);
poly* inverse_poly(poly* p);
poly* error_correction(poly* roots, poly* S);


//field math
int gf_mult(int a, int b);
int gf_div(int a, int b);
int setup_tables();
int gf_pow(unsigned int a, int b);
int gf_inverse(int num);