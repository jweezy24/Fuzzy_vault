#include "./headers/reedSolo.h"

long p = pow(2,16);

int deg = 6;

char* mesg = "ABCDEF";

int eval = 256;
int NW = 1 << 16;
unsigned short *gflog, *gfilog;

int setup_tables(){
    unsigned int x_to_w, b, log, prim_poly;

    x_to_w =  1 << 16;
    gflog = malloc(sizeof(unsigned short) * x_to_w);
    gfilog = malloc(sizeof(unsigned short) * x_to_w);
    prim_poly = 0210013;

    b = 1;
    for(log = 0; log < x_to_w-1; log++){
        gflog[b] = (unsigned short)log;
        gfilog[log] = (unsigned short) b;
        b = b << 1;
        if(b & x_to_w) b = b ^ prim_poly;

    }

    // for(int i =0; i < x_to_w-1; i++){
    //     printf("\n%d\n", gflog[i]);
    // }

    return 0;

}

int gf_mult(int a, int b){
    int sum_log;

    if(a == 0 || b == 0) return 0;
    sum_log = gflog[a] + gflog[b];
    if(sum_log >= NW-1) sum_log -= NW-1;
    return gfilog[sum_log];
}

int gf_div(int a, int b){
    int diff_log;

    if(a == 0) return 0;
    if(b == 0) return -1;
    diff_log = gflog[a] - gflog[b];
    if(diff_log < 0) diff_log += NW-1;
    return gfilog[diff_log];
}


int g(int x){
    int ret = 1;
    int generator = 5;
    for(int i = 1; i < eval; i++){
        ret *= (x-pow(generator, i));
    }
    return (ret)%p;
}


double* b(double* m, int k ){
    gsl_matrix* X = gsl_matrix_alloc(k,k);
    gsl_vector* a = gsl_vector_alloc(k);
    gsl_vector* y = gsl_vector_alloc(k);


    //fill y vector
    for(int i = 0; i < k; i++){
        unsigned int entry = 0;
        for(int j =0; j < k; j++){

            unsigned int num_tmp = 1;
            for(int l = 0; l < 32; l++){
                num_tmp = gf_mult(num_tmp, j);
                //printf("\n%d\n", num_tmp);
            }
            int num_tmp_2 = 1;
            for(int s = 0; s < k-i; s++){
                num_tmp_2 = gf_mult(num_tmp_2, j);
            }
            num_tmp = gf_mult(gf_mult((unsigned int)m[i], num_tmp_2), num_tmp);
            //printf("%d\n", num_tmp);
            entry ^= num_tmp;
        }
            gsl_vector_set(y, i, entry);
    }

    //fill X matrix
    for(int i =0; i < k; i++){
        for(int j = 0; j < k; j++){
            if(j == 0){
                gsl_matrix_set(X, i,j, 1);
            }else{
                int num_tmp = 1;
                for(int k = 0; k < j; k++){
                    num_tmp = gf_mult(num_tmp, i);
                }
                // printf("\n%d\n", num_tmp);
                gsl_matrix_set(X, i,j, (long int)num_tmp);
            }
        }
    }

    return create_coeff_mat(X, a, y, k);


}

double* c(double* m, double* b, int k){
    gsl_matrix* X = gsl_matrix_alloc(k,k);
    gsl_vector* a = gsl_vector_alloc(k);
    gsl_vector* y = gsl_vector_alloc(k);

        //fill y vector
    for(int i = 0; i < k; i++){
        unsigned int entry = 0;
        for(int j =0; j < k; j++){

            unsigned int num_tmp = 1;
            for(int l = 0; l < 32; l++){
                num_tmp = gf_mult(num_tmp, j);
                //printf("\n%d\n", num_tmp);
            }
            int num_tmp_2 = 1;
            for(int s = 0; s < k-i; s++){
                num_tmp_2 = gf_mult(num_tmp_2, j);
            }

            int num_tmp_3 = 1;
            for(int s = 0; s < k-i; s++){
                num_tmp_3 = gf_mult(num_tmp_3, j);
            }
            num_tmp_3 = gf_mult((unsigned int)b[i], num_tmp_3);
            num_tmp = gf_mult(gf_mult((unsigned int)m[i], num_tmp_2), num_tmp);
            //printf("%d\n", num_tmp);
            entry ^= (num_tmp ^ num_tmp_3);
        }
            gsl_vector_set(y, i, entry);
    }

    for(int i =0; i < k; i++){
        for(int j = 0; j < k; j++){
            if(j == 0){
                gsl_matrix_set(X, i,j, 1);
            }else{
                int num_tmp = 1;
                for(int k = 0; k < j; k++){
                    num_tmp = gf_mult(num_tmp, i);
                }
                // printf("\n%d\n", num_tmp);
                gsl_matrix_set(X, i,j, (long int)num_tmp);
            }
        }
    }

    return create_coeff_mat(X, a, y, k);

}

gsl_matrix* create_inverse(gsl_matrix* X){
    int size = deg;
    int tmp = 0;
    int inverse = 0;
    int k = 0;

    gsl_matrix* copy = gsl_matrix_alloc(size, size);

    gsl_matrix* inv = gsl_matrix_alloc(size, size);

    gsl_matrix_memcpy(copy, X);

    for (int i = 0; i < size; ++i) {
        //Grabbing ith element on the diagonal
        tmp = gsl_matrix_get(copy, i, i);
        if(tmp != 1){
            inverse = gf_div(1, tmp);
            for (int j = 0; j < size; ++j) {
                int element = gsl_matrix_get(copy, i, j);

                gsl_matrix_set(copy, i,j, gf_mult(element, inverse));
                gsl_matrix_set(inv, i,j, gf_mult(element, inverse));
            }
        }

      for (int j = i+1; j < size; j++) {
        if (gsl_matrix_get(copy, i,j) != 0) {
            if (gsl_matrix_get(copy, i,j) == 1) {
                for (int x = i; x < size; x++) {
                    int tmp_1, tmp_2; 
                    tmp_1 = (int)gsl_matrix_get(copy, i, j) ^ (int)gsl_matrix_get(copy, i, x);
                    tmp_2 = (int)gsl_matrix_get(inv, i, j) ^ (int)gsl_matrix_get(inv, i, x);
                    gsl_matrix_set(copy, i, j, tmp_1);
                    gsl_matrix_set(inv, i, j, tmp_2);
                }
            } else {
            tmp = gsl_matrix_get(copy, i, j);
                for (int x = i; x < size; x++) {
                    int tmp_1, tmp_2; 
                    tmp_1 = (int)gsl_matrix_get(copy, i, j) ^ (int)gf_mult(tmp, gsl_matrix_get(copy, i, x));
                    tmp_2 = (int)gsl_matrix_get(inv, i, j) ^ (int)gf_mult(tmp, gsl_matrix_get(inv, i, x));
                    gsl_matrix_set(copy, i, j, tmp_1);
                    gsl_matrix_set(inv, i, j, tmp_2);
                }
            }
        }
    }

    for(int i = size-1; i >= 0; i--){
        for(int j = 0; j < i; j++){
            if(gsl_matrix_get(copy, j, i) != 0){
                tmp = gsl_matrix_get(copy, j, i);
                gsl_matrix_set(copy, j,i,0);
                for(int k = 0; k < size; k++){
                    int tmp_1 = gf_mult(tmp, (int)gsl_matrix_get(inv, i, k)) ^ (int)gsl_matrix_get(inv, k,i); 
                    gsl_matrix_set(inv, k,i, tmp_1); 
                }

            }
        }

    }
    }

    gsl_matrix_free(copy);
    return inv;
}



double* create_coeff_mat(gsl_matrix* X, gsl_vector* a, gsl_vector* y, int k){
    double* coeff = malloc(sizeof(unsigned int)*k);
    //print_mat_contents(X);
    gsl_matrix* X_inv = create_inverse(X);
    //print_mat_contents(X_inv);

    for(int i =0; i < k; i++){
        gsl_vector* row_tmp = gsl_vector_alloc(k);
        gsl_matrix_get_col(row_tmp, X_inv, i);
        gsl_vector_mul(row_tmp, y);
        for(int l = 0; l < k; l++){
            if(i == 0){
                printf("%d\n", row_tmp->data[l]);
            }
            coeff[i] = fmod((double)((unsigned int)coeff[i]%p ^ (unsigned int)(row_tmp->data[l])%p),p);
        }
    }

    return coeff;
}


//Vandermonde Matrix Method
//https://en.wikipedia.org/wiki/Vandermonde_matrix
double* get_m(char* mesg, int k){

    gsl_matrix* X = gsl_matrix_alloc(k,k);
    gsl_vector* a = gsl_vector_alloc(k);
    gsl_vector* y = gsl_vector_alloc(k);

    int message_len = getlen(mesg);
    
    //fill y vector with chars
    for(int i = 0; i < message_len; i++){
        gsl_vector_set(y, i, mesg[i]);
    }

    //fill X matrix
    for(int i =0; i < k; i++){
        for(int j = 0; j < k; j++){
            if(j == 0){
                gsl_matrix_set(X, i,j, 1);
            }else{
                int num_tmp = 1;
                for(int k = 0; k < j; k++){
                    num_tmp = gf_mult(num_tmp, i);
                }
                // printf("\n%d\n", num_tmp);
                gsl_matrix_set(X, i,j, (int)num_tmp);
            }
        }
    }

    return create_coeff_mat(X, a,  y, k);

}   

int getlen(char* mesg){
    int count =0;
    while(mesg[count] != 0){
        count +=1;
    }
    return count;
}

void print_mat_contents(gsl_matrix *matrix)
{
    int size = deg;
    size_t i, j;
    double element;

    for (i = 0; i < size; ++i) {
        for (j = 0; j < size; ++j) {
            element = gsl_matrix_get(matrix, i, j);
            printf("%f ", element);
        }
        printf("\n");
    }
}


/*
n = block size of message
k = message size
q = symbol in bit size
mesg = message we want to encode

*/
int* reed_encoding(int n, int k, int q, char* mesg){


}

void main(){
    setup_tables();
    double* m = get_m(mesg, deg);


    for(int i = 0; i < deg; i++){
        printf("%f*x^%d + \t", m[i], deg-i);
    }
    printf("\n");



    double* b1 = b(m, deg);

    for(int i = 0; i < deg; i++){
        printf("%f*x^%d + \t", b1[i], deg-i);
    }
    printf("\n");

    double* c1 = c(m, b1, deg);

    for(int i = 0; i < deg; i++){
        printf("%f*x^%d + \t", c1[i], deg-i);
    }
    printf("\n");


}