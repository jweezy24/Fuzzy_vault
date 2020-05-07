#include <stdio.h>
#include <stdlib.h>



long prime=6700417;
int t=5;

int getlen(char* mesg);
int** machine1(int k, int t, int r, char* mesg);
int f(int deg, char x, int* coeff);
int pow_jack(int val, int amount);
int is_proper_element(int x,int y);
void unlock(int k, int t, int r, int** R);
int** coeff_calculation_Lagrange(int layer, int k, int** new_k, int** tmp_k, int len, int len_inner);
int* polynomial_generation(int deg);