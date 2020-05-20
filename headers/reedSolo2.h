#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

//Field math defs
int gf_mult(int a, int b);
int gf_div(int a, int b);
int setup_tables();
int gf_pow(int a, int b);
int* gf_mult_poly(int* a, int* b, int a_len, int b_len);
int* gf_div_poly(int* a, int* b, int a_len, int b_len, int remainder);
int* gf_poly_add(int* a, int* b, int len, int len2);
int gf_inverse(int num);
int eval_poly(int* poly, int len, int x);

//utils
void bin(unsigned n); 
unsigned int str_int(char* str);
void print_arr(int* a, int len );
int* x_pow(int pow);

//Reed Solomon funcs
int* g(int t);
int* syndome_calculator_collective_sums(int* C, int len_c, int t);
int* syndome_calculator_division(int* C, int* g, int len_c, int len_g);
int parity_check(int* C, int* g, int len);
void decode(int* C, int k, int n);
int* error_locating_poly(int* S, int t, int n);
int calculate_S(int* R, int num, int n);
int** berlecamp_table(int* R, int t, int n);
int* sigma_r(int* s, int len);

