#include <stdio.h>
#include <stdlib.h>
#include <math.h>


typedef struct polynomial{
    int* coeffs;
    int size;
}poly;

long p = pow(2,16);
int symbols = 256;
unsigned int NW = 1 << 16;
unsigned int *gflog, *gfilog;
unsigned int prim_poly = (unsigned int) 0b10000001111011101;

//utils
void bin(unsigned n);
void print_arr(int* a, int len );
int deg(int* poly, int size);

//polynomial data structure functions
poly* create_poly(int* coeff, int len);


//field math
int gf_mult(int a, int b);
int gf_div(int a, int b);
int* gf_div_poly(int* a, int* b, int a_len, int b_len, int remainder);