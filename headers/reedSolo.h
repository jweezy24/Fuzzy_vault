#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <gsl/gsl_matrix_double.h>
#include <gsl/gsl_linalg.h>
#include <math.h>


gsl_matrix *invert_a_matrix(gsl_matrix *matrix);
int* g();
double* get_m(char* mesg, int k);
int* polynomial_generation(int n, int k);
int* lagrange_interpolation(char* mesg, int k);
int getlen(char* mesg);
int* reed_encoding(int n, int k, int t);
double* create_coeff_mat(gsl_matrix* X, gsl_vector* a, gsl_vector* y, int k);
double* b(double* m, int k );
int gf_mult(int a, int b);
int gf_div(int a, int b);
int setup_tables();
gsl_matrix* create_inverse(gsl_matrix* X);
void print_mat_contents(gsl_matrix *matrix);
int gf_pow(int a, int b);
int* gf_mult_poly(int* a, int* b, int a_len, int b_len);
int* m_(char* mesg, int n, int k);
int* gf_div_poly(int* a, int* b, int a_len, int b_len, int remainder);
int* syndrome_calculation(int* c, int n,int t);