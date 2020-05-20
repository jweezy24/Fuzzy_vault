#include "./headers/reedSolo.h"

long p = pow(2,16);

int deg = 7;

char* mesg = "ABCDEF";

int symbols = 256;
int NW = 1 << 16;
unsigned int *gflog, *gfilog;
unsigned int prim_poly = 0210013;

int setup_tables(){
    unsigned int x_to_w, b, log;

    x_to_w =  p;
    gflog = malloc(sizeof(unsigned int) * x_to_w);
    gfilog = malloc(sizeof(unsigned int) * x_to_w);

    b = 1;
    for(log = 0; log < x_to_w-1; log++){
        gflog[b] = (unsigned int)log;
        gfilog[log] = (unsigned int) b;
        b = b << 1;
        if(b & x_to_w) b = b ^ prim_poly;

    }

    // for(int i =0; i < x_to_w-1; i++){
    //     printf("%d\n", gflog[i]);
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

int* gf_mult_poly(int* a, int* b, int a_len, int b_len){

    int new_poly_size = a_len+b_len -1;

    int* new_poly = malloc(sizeof(int)*new_poly_size);
    
    for(int i =0; i < new_poly_size; i++){
        new_poly[i]=0;
    }

    for(int i = a_len-1; i >= 0; i--){
        for(int j = b_len-1; j >= 0; j--){
            new_poly[i+j] ^= gf_mult(a[i], b[j]);
            
        }
    }

    return new_poly;
}

int gf_div(int a, int b){
    int diff_log;

    if(a == 0) return 0;
    if(b == 0) return -1;
    diff_log = gflog[a] - gflog[b];
    if(diff_log < 0) diff_log += NW-1;
    return gfilog[diff_log];
}

int* gf_poly_add(int* a, int* b, int len, int len2){
    int* poly = malloc(sizeof(int)*len);
    for(int i =0; i < len; i++){
        if(i >= len2){
            poly[i] = a[i] ^ 0;
        }else{
            poly[i] = a[i] ^ b[i];
        }
    }
    return poly;
}

// a/b if remainder = 1 return remainder else return the divsion
int* gf_div_poly(int* a, int* b, int a_len, int b_len, int remainder){
   
    // if(a_len < b_len){
    //     if(remainder){
    //         return b;
    //     }else{
    //         return 0;
    //     }
    // }

    int range = a_len - b_len - 1;

    int* divisor = malloc(sizeof(int)*b_len);

    int* dividend = malloc(sizeof(int)*a_len);

    for(int i =0; i < b_len; i++){
        divisor[i] = b[i];
 
    }
    for(int i =0; i < a_len; i++){
        dividend[i] = a[i];
 
    }

    int* r = malloc(sizeof(int)*(a_len));

    for(int i = a_len-1; i >= range; i--){
        int coeff = dividend[i];

        if(coeff != 0){
            for(int j = i; j > 0; j--){
                if (divisor[j] != 0){
                    //printf("a=%d  b=%d b/a=%d range=%d\n", coeff, divisor[j], gf_div(coeff, divisor[j]), range);
                    dividend[j] ^= gf_mult(divisor[j-i], coeff);
                }
            }
        }

    }



    int sep = b_len - 1;

    for(int i = 0; i < sep; i++){
        r[i] = dividend[i];
        dividend[i] = 0;
    }

    if(remainder == 1){
        return r;
    }else{
        return dividend;
    }

}

int gf_pow(int a, int b){
    int ret = 1;
    for (int i =0; i < b; i++){
        ret = gf_mult(ret, a);
    }
    return ret;
}


void check_generator(){
    unsigned int generator = 0210013;
    int tmp = 0;
    for(int j =2; j < NW; j++){
        generator = j;
        for(int i = 1; i < NW; i++){
            generator = gf_pow(generator, i);
            if(generator == 1){
                if(i == 257){
                    printf("i = %d, j = %d\n", i, j);
                }
                break;
            }
        }
    }

    printf("Finished\n Generator =  %d\n", generator);

}

int* g(int t){
    //int* ret = malloc(sizeof(int)* (p-1));
    int generator = 0210013;
    int* tmp_1 = malloc(sizeof(int)*2);
    tmp_1[0] = generator;
    tmp_1[1] = 1;
    int size = 2;
    for(int i = 1; i < 2*t; i++){
        int* tmp_2 = malloc(sizeof(int)*2);
        tmp_2[0] = gf_pow(generator, i+1);
        tmp_2[1] = 1;
        int* holder = gf_mult_poly(tmp_1, tmp_2, size, 2);
        size = 1+size;
        tmp_1 = realloc(tmp_1, size*sizeof(int));
        for(int j =0; j < size; j++){
            tmp_1[j] = holder[j];
        }
    }

    printf("size = %d 2*t = %d\n", size, 2*t);

    // for(int i = 0; i < size; i++){
    //     if(i == 0){
    //         printf("%d\t +", tmp_1[i]);
    //     }else{
    //         printf("%d\t +", tmp_1[i]);
    //     }
    // }
    // printf("\n");
    return tmp_1;
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
                num_tmp = gf_pow(prim_poly, j);
                // printf("\n%d\n", num_tmp);
                gsl_matrix_set(X, i,j, (long int)num_tmp);
            }
        }
    }

    return create_coeff_mat(X, a, y, k);


}

int* c(double* m, double* b, int k){
    int* coeff = malloc(sizeof(int)*2*k);
    for(int i = 0; i < k; i++){
        coeff[i] = (int)m[i];
    }
    for(int j = k ; j < 2*k; j++){
        coeff[j] = (int)b[j-k];
    }
    return coeff;
}

double* c_2(double* m, double* b, int k){
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
                    num_tmp = gf_mult(num_tmp, prim_poly);
                }
                // printf("\n%d\n", num_tmp);
                gsl_matrix_set(X, i,j, (int)num_tmp);
            }
        }
    }

    return create_coeff_mat(X, a,  y, k);

}   

void div_mesg_g(char* mesg){
    int size = getlen(mesg);
    unsigned int *divisor_poly = malloc(sizeof(unsigned int)* size);
    unsigned int *mesg_copy = malloc(sizeof(unsigned int)*size);

    for(int i =0; i < size; i++){
        mesg_copy[i] = mesg[i];
    }

    for(int i =0; i < size; i++){
        unsigned int coeff = mesg_copy[i];
        unsigned int pp = prim_poly;
        unsigned int divisor = 1;

        while(coeff != pp){
            
        }
    }
}

int getlen(char* mesg){
    int count =0;
    while(mesg[count] != 0){
        count +=1;
    }
    return count;
}

void print_mat_contents(gsl_matrix *matrix){
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

int* m_(char* mesg, int n, int k){
    int* x = malloc(sizeof(int)* (n-k)+1);
    int len = getlen(mesg);
    int* mesg_2 = malloc(sizeof(int)*len);


    for(int i = 0; i < len; i++){
        mesg_2[i] = (int)mesg[i];
    }

    for(int i = 0; i < (n-k); i++){
        if(i == (n-k)-1){
            x[i] = 1;
        }else{
            x[i] = 0;
        }
    }

    return gf_mult_poly(x, mesg_2, n-k, len);


}


/*
n = block size of message
k = message size
q = symbol in bit size
mesg = message we want to encode

*/
int* reed_encoding(int n, int k, int t){
    
    setup_tables();
    int* g_x = g(t);
    int* m = m_(mesg, n, k);
    //check_generator();

    for(int i = 0; i < n; i++){
        printf("%d*x^%d + \t", m[i], i);
        
    }
    printf("\n");
    for(int i = 0; i < n; i++){
        printf("%d*x^%d + \t", g_x[i], i);
        
    }
    printf("\n");
    
    
    int* ck = gf_div_poly(m,g_x, n, n-k, 0);
    int* r = gf_div_poly(m,g_x, n, n-k, 1);

    int* c = gf_poly_add(ck, r, n, n-k);

    int* test = gf_div_poly(c,g_x, n, n-k, 1);
    int len = (n-k - (n-k-t));
    
    
    for(int i = 0; i < n; i++){
        printf("%d*x^%d + \t", c[i], i);
        
    }
    printf("\n");
    for(int i = 0; i < n; i++){
        printf("%d*x^%d + \t", r[i], i);
        
    }
    printf("\n");


    int* c_3 = gf_div_poly(c, g_x, n, n-k, 0);

    int* c_2 = gf_div_poly(c, g_x, n, n-k, 1);
    
    int* c_4 = gf_poly_add(c_2, c_3, n, n-k);

    for(int i = 0; i < n; i++){
        printf("%d*x^%d + \t", c[i], i);   
    }
    printf("\n");

    int* R_x = syndrome_calculation(c,n,t);

    int* m_2 = gf_div_poly(R_x,g_x, n, n-k, 0);

    int* m_dec_2 = gf_div_poly(c,g_x, n, n-k, 0);

    // for(int i = 0; i < n; i++){
    //     printf("%d*x^%d + \t", m_2[i], i);   
    // }
    // printf("\n");

    //int* dec_1 = gf_div_poly(c, g_x, n, t, 0);
    //int* dec_2 = gf_div_poly(c, g_x, n, t, 1);

    //int* dec_3 = gf_poly_add(dec_1, g_x, n);



    // for(int i = 0; i < n; i++){
    //     printf("%d + \t", dec_2[i], i);   
    // }
    // printf("\n");


    // double* b1 = b(m, deg);

    // for(int i = 0; i < deg; i++){
    //     if(i != deg-1){
    //         printf("%f*x^%d + \t", b1[i], deg-(i+1));
    //     }else{
    //         printf("%f\n", b1[i]);
    //     }
    // }
    // int* c1 = c(m, b1, deg);

    // for(int i = 0; i < deg*2; i++){
    //     if(i != deg*2-1){
    //         printf("%d*x^%d + \t", c1[i], 2*deg-(i+1));
    //     }else{
    //         printf("%d\n", c1[i]);
    //     }
    // }

    // syndrome_calculation(c1);

}

int* syndrome_calculation(int* c, int n,int t){

    int syndrome = 0;
    unsigned int prim_poly = 0210013;
    int* R = malloc(sizeof(int)*n);

    for(int i = 0; i < 2*t; i++){
        for(int j = 0; j < n; j++){
            syndrome ^= gf_mult(c[j], gf_pow(gf_pow(prim_poly, i), j));
        }

        R[i] = syndrome;    
        syndrome = 0;
    }

    return R;

}

void main(){
    int n = 256;
    int k = 6;
    int d = n-k+1;
    int t = (n-k)/2;

    reed_encoding(n,k,t);

}