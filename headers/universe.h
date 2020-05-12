#include <stdio.h>
#include <stdlib.h>
#include "polifitgsl.h"



long prime=6700417;
int t=5;

int getlen(char* mesg);
void machine1(int k, double** axis);
int f(int x);
int pow_jack(int val, int amount);
int is_proper_element(double x, double y, int len, double** B);
void unlock(int** R, double** B, int k);
int stream_len();
double** create_domain_range();
int str_int(char* str);
double** create_domain_range_corruption();