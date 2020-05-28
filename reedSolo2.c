#include "./headers/reedSolo2.h"

char* mesg = "Hello";
long p = pow(2,16);
int symbols = 256;
unsigned int NW = 1 << 16;
unsigned int *gflog, *gfilog;
extern char bits[];
unsigned int prim_poly = (unsigned int) 0b10000001111011101;
int generator = (unsigned int)0b1000011;
//unsigned int prim_poly = 0b;

int* g(int t){
    int num = 0;
    int count = 0;

    // for(int i = 1; i < NW; i++){
    //     count = 0;
    //     num = 0;
    //     for(int j = 1; j < NW; j++){
    //         num = gf_pow(i, j);
    //         if(num == 1){
    //             //printf("i = %d\t p = %d\t j = %d\n", i, p, j);
    //             break;
    //         }
    //         count+=1;
    //     }

    //     //if (count >= NW-2){
    //     printf("i = %d\n",i);
    //     //} 
        

    // }
    int* tmp_1 = malloc(sizeof(int)*2);
    tmp_1[0] = generator;
    tmp_1[1] = 1;
    int size = 2;
    for(int i = 1; i < 2*t; i++){
        int* tmp_2 = malloc(sizeof(int)*2);
        tmp_2[0] = gf_pow(generator, i+1);
        tmp_2[1] = 1;
        int* holder = gf_mult_poly(tmp_1, tmp_2, size, 2);
        size = 2+size+1;
        tmp_1 = realloc(tmp_1, size*sizeof(int));
        for(int j =0; j < size; j++){
            tmp_1[j] = holder[j];
        }
        free(tmp_2);
    }
    printf("%d\n", size);
    return tmp_1;
}

//encoding chars
int* m_charp(int n, int k, int t){
    int * M = malloc(sizeof(int)*n);
    int pos_holder = n-1;
    
    for(int i =0; mesg[i] != 0; i++){
        int letter = mesg[i];

        M[pos_holder] = letter;
        pos_holder-=1;
        
    }

    for(int i = pos_holder; i >=0; i--){
        M[i] = 0;
    }
    return M;
}

//encoding bits
int* m_(int n, int k, int t){
    int * M = malloc(sizeof(int)*n);
    char* tmp = malloc(sizeof(char)*9);
    int pos_holder = n-1;
    int bits_pos = 0;
    int count = 0;
    
    for(int i =0; i <= k*8; i++){
        if(count%8 == 0 && count > 0){
            unsigned int tmp_h = (unsigned int)str_int(tmp);
            M[pos_holder] = tmp_h;
            pos_holder--;
            count = 0;
            i--; 
        }else{
            tmp[count] = bits[bits_pos];
            count+=1;
            bits_pos+=1;
        }
        
    }

    for(int i = pos_holder; i >=0; i--){
        M[i] = 0;
    }

    free(tmp);
    return M;
}

int* syndome_calculator_collective_sums(int* C, int len_c, int t){
    int* Synd = malloc(sizeof(int)* 2*t);
    for(int i =0; i < 2*t; i++){
        int S_i = 0;
        int alpha = gf_pow(generator, i);
        for(int j = 0; j< len_c; j++){
            S_i ^= gf_mult(C[j], gf_pow(alpha, j));
        }
        Synd[i] = S_i;
    }
    return Synd;
}

int* syndome_calculator_division(int* C, int* g, int len_c, int len_g){

    return gf_div_poly(C, g, len_c, len_g, 1);

}



int* encode_message(int n, int k, int t){
    int* g_x = g(t); 
    //int* g_x = 0;
    int* M = m_(n, k, t);


    time_t dd;
    srand((unsigned) time(&dd));

    //int* Q = gf_div_poly(M, g_x, n, n-k,0);
    int* CK = gf_div_poly(M, g_x, n, n-k,1);

    int* C = gf_poly_add(M,CK, n, n-k);

    //int* S_i_d = syndome_calculator_division(C, g_x, n, n-k);

    //print_arr(C, n);

    //C is the original message, the for loop below will edit the loop as if to add random noise.
    for(int i = 0; i < n; i++){
        //5% chance of error
        //printf("%d\n", rand());
        if(rand()%100 > 95){
            unsigned int val = rand()%100;
            C[i] = C[i] ^ val;
            printf("ERROR PUT IN COEFF %d ERROR OFF BY %d\n", i, val);
        }
    }

    //BERLECAMP's ALGORITM
    // int** sigmas = berlecamp_table(C, t, n);

    // int* s = sigmas[2*t+1];
    // int size = sigmas[2*t+2][2*t+1];

    // int* s_r = sigma_r(s, size);

    // int* roots = roots_of_poly(s, size, n);

    // int total_roots = roots[0];

    // // for (int i =0; i < total_roots; i++){
    // //     if(roots[i] == 0 ){
    // //         printf("%u\n", roots[i]);
    // //     }
    // // }


    int* S = syndome_calculator_division(C,  g_x, n, n-k);
    

    int* s_r = euclid_alg(S, t);

    //print_arr(s, 2*t);

    if(s_r != 0){
        // // int* sig_d = gf_div_poly(sig, x_pow(6), 2*n, 7, 0);
        
        //int* s_r = sigma_r(s, t);

        int* roots = roots_of_poly(s_r, 2*t, t, n);

        //printf("Printing SXSig/S\n");
        //print_arr(s, 2*t);
        printf("Printing Sig_R\n");
        print_arr(s_r, 2*t);
        printf("Printing Roots\n");
        print_arr(roots, n);
    }

    return C;
}


int* sigma_r(int* s, int t){
    int pos = 0;
    int size = (deg(s,2*t)+1);
    int* sigma_r = malloc(sizeof(int)*size);
    printf("SIZE of sigma_r=%d", size);
    for(int i =size-1; i >=0; i--){
        sigma_r[pos] = s[i];
        pos++;
        
    }
    return sigma_r;
}

int* roots_of_poly(int* sigma_r, int len, int t, int n){
    int* zeros_ = malloc(sizeof(int)*n);
    int zeros = 0;
    unsigned int num = 0;
    zeros_[0] = 0;

    for(int i=0; i < n; i++){
        //num = (unsigned int) eval_poly(sigma_r, len, gf_pow(generator, i));
        printf("%d\n", num);
        if(num == 0){
            zeros_[zeros] = i;
            zeros++;
            printf("ZERO AT %d\n", i);
        }
        //printf("%d\n", zeros);
    }


    // for(int i=1; i < NW; i++){
    //     num = (unsigned int) eval_poly(sigma_r, len, i);
    //     if(num == 0){
    //         zeros_[zeros] = i;
    //         zeros_ = realloc(zeros_, sizeof(int)*(zeros+1));
    //         zeros++;
    //         printf("%d\n", i);
    //     }
    //     //printf("%d\n", zeros);
    // }

    zeros_[0] = zeros;

    return zeros_;
}

//Numeric calculation
int* create_S_poly(int* R, int t, int n){
    int* S = malloc(sizeof(int)*2*t);

    for(int i = 0; i < 2*t; i++){
        S[i] = calculate_S(R, i, n);
        
    }

    print_arr(S, n);
    return S;
}


int** berlecamp_table(int* R, int t, int n){

    int** b_table = malloc(sizeof(int*)* (2*t+2));
    int** sigmas = malloc(sizeof(int*)*(2*t+3));
    int* sigma_sizes = malloc(sizeof(int)*(2*t+2));


    int r1[5] = {-1, 1, 1, 0, 1};
    int tmp = calculate_S(R, 1, n);
    int r2[5] = {0, 1, 1, tmp, 0};
    
    int* simga_r1 = malloc(sizeof(int));
    int* simga_r2 = malloc(sizeof(int));

    for(int i = 0; i < 1; i++){
        if(i == 0){
            simga_r1[i] = 1;
            simga_r2[i] = 1;
        }else{
            simga_r1[i] = 1;
            simga_r2[i] = 1;
        }
    }
    sigmas[0] = simga_r1;
    sigmas[1] = simga_r2;

    sigma_sizes[0] = 1;
    sigma_sizes[1] = 1;

    b_table[0] = r1;
    b_table[1] = r2;

    for(int i = 2; i < 2*t+2; i++){
        int* row_before = b_table[i-1];
        int* curr_row = malloc(sizeof(int)*5);
        
        int mu = i;
        
        int d_mu_b = calculate_S(R, i-1, n);
        int d_mu = calculate_S(R, i, n);
        
        int* sigma_mu_b = sigmas[i-1];
        //int* sigma_mu = malloc(sizeof(int)*2*t);

        int h_mu_b = row_before[4];
        int h_mu = 0;

        if(d_mu != 0){
            int max_h = h_mu_b;
            int max_h_pos = 0;
            int h_sums = 0;
            int* sigma_row;
            for(int j = i-1; j >= 0; j--){
                int* row = b_table[j];
                int h_mu_j = row[4];
                h_sums = h_mu_j ^ abs(i-1) ^ abs(j-1);
                if (h_sums > max_h){
                    max_h = h_sums;
                    max_h_pos = j;
                    sigma_row = sigmas[j];
                }
            }
            int d_row = b_table[max_h_pos][4];
            int d_row_inv = gf_inverse(d_row);
            unsigned int coeff = gf_mult(d_mu_b, d_row_inv);
            int pow = i ^ abs(max_h_pos-1);
            int* new_x = x_pow(pow);
            int* new_sig = gf_mult_poly(new_x, sigma_row, pow, sigma_sizes[max_h_pos]);
            int new_size = pow+2*t - 1;
            sigma_sizes[i] = new_size;
            int* final_sig =  gf_poly_add(sigma_mu_b , new_sig, sigma_sizes[max_h_pos], new_size);
            print_arr(final_sig, 2*t);
            sigmas[i] = final_sig;
            curr_row[0] = abs(i-2);
            curr_row[1] = 1;
            if(mu-1 > 0){
                curr_row[2] = calculate_S(R, mu+1, n) ^ eval_poly(final_sig, new_size, 1);
            }else{
                curr_row[2] = calculate_S(R, mu+1, n) ^ eval_poly(final_sig, new_size, calculate_S(R, mu-1, n));
            }
            curr_row[3] = max_h;
            curr_row[4] = mu ^ max_h;
            b_table[i] = curr_row;
        }else{
            sigmas[i] = sigma_mu_b;
            sigma_sizes[i] = sigma_sizes[i-1];
            h_mu = h_mu_b;
            curr_row[0] = i-2;
            curr_row[1] = 1;
            if(mu-1 > 0){
                curr_row[2] = calculate_S(R, mu+1, n) ^ eval_poly(sigma_mu_b, sigma_sizes[i-1], 1);
            }else{
                curr_row[2] = calculate_S(R, mu+1, n) ^ eval_poly(sigma_mu_b, sigma_sizes[i-1], calculate_S(R, mu-1, n));
            }
            curr_row[3] = h_mu;
            curr_row[4] = mu ^ h_mu; 
        }

    }

    // for(int i = 0; i < 2*t+2; i++){
    //    printf("HERE\n");
    //    print_arr(sigmas[i], sigma_sizes[i]);
    // }

    // for(int i =0; i < 2*t+2; i++){
    //     free(b_table[i]);
    // }

    free(b_table);
    sigmas[2*t+2] = sigma_sizes;

    return sigmas;

}


int* euclid_alg(int* S, int t){
    int* x_2t = x_pow(2*t);

    int pos = 0;

    print_arr(x_2t, 2*t+1);
    print_arr(S, 2*t);

    int degree = deg(S, 2*t);
    printf("DEGREE=%d\n", deg(S, 2*t));
    int** euc;

    if(degree == 0 && S[0] == 0){
        return 0;
    }else{
        euc = euclid_alg_rec(x_2t, S, t, 2*t+1, 2*t);
    }

    int* y = euc[1];
    int* y_2 = malloc(sizeof(int)* (deg(y, 2*t)+1));

    printf("Printing SXSig degree of 0,1=%d,%d\n", deg(euc[0],2*t)+1, deg(euc[1],2*t)+1);
    int y_size = deg(euc[0],2*t)+deg(euc[1],2*t)+1;
    printf("Printing y y_size=%d\n", y_size);
    print_arr(y, deg(y, 2*t)+1);
    for(int i =deg(y, 2*t); i >= 0; i--){
        
        y_2[pos] = y[i];
        pos++;
        
    }
    for(int i = pos; i < 2*t;i++){
        y_2[i] = 0;
    }
    // int* q = gf_div_poly(y, S, y_size, deg(S, 2*t)+1, 0);
    // int* r = gf_div_poly(y, S, y_size, deg(S, 2*t)+1, 1);
    
    // int* sig = gf_poly_add(q,r, deg(q,2*t)+1, deg(r, 2*t)+1);
    // printf("printing sigma.\n");
    // print_arr(sig, deg(sig, 2*t)+1);


    return y_2;
}

int** euclid_alg_rec(int* a, int* b, int t, int a_len, int b_len){
    
    printf("Printing Polnomial A\n");
    print_arr(a, deg(a,a_len)+1); 
    printf("Printing Polnomial B\n");
    print_arr(b, deg(b,b_len)+1);
    int degree = 0;

    if(a_len > 1){
        degree = deg(a, a_len);
    }else{
        degree = 0;
    }
    printf("DEGREE IN REC ALG=%d\n", degree);
    if (degree <= t){
        int** ret_val = malloc(sizeof(int*)*3);
        int* zero = malloc(sizeof(int)*2*t);
        int* b2 = malloc(sizeof(int)*2*t);
        for(int i = 0; i < 2*t; i++) {zero[i] =0;}
        for(int i = 0; i < 2*t; i++) {
            if(i == 0){
                b2[i]= 1;
            }else{
                b2[i]= 0;
            }
        }
        zero[0] = a[0];
        ret_val[0] = b;
        ret_val[1] = zero;
        ret_val[2] = b2;
        return ret_val;
    }else{

        printf("INNER DEGREE A = %d\n", degree);
        
        
        int* a2 = gf_div_poly(a, b, deg(a,a_len)+1, deg(b,b_len)+1,1);
        printf("Printing polynomial A2\n");
        print_arr(a2, deg(a2, 2*t)+1);
        printf("Printing polynomial B inside.\n");  
        print_arr(b, deg(b, 2*t)+1);
        int** tmp_holder = euclid_alg_rec(b, a2, t, deg(b, b_len)+1, deg(a2, a_len)+1);
        
        int* gcd = tmp_holder[0];
        int* x1 = tmp_holder[1];
        int* y1 = tmp_holder[2];

        printf("Printing X1 and X2\n");
        int error_msg_x = print_arr(x1, deg(x1, 2*t)+1);
        int error_msg_y = print_arr(y1, deg(y1, 2*t)+1);


        if(error_msg_x == 1 && error_msg_y == 1){
            print_arr(a, deg(a, a_len)+1);
            print_arr(b, deg(b, 2*t)+1);
            printf("BOTH GOOD\n");
            int* q = gf_div_poly(b, a, deg(b,b_len)+1, deg(a,a_len)+1,0);
            printf("PRINTING Q\n");
            print_arr(q, deg(q, 2*t)+1);
            printf("PRINTING X1\n");
            print_arr(x1, deg(x1, 2*t)+1);
            int* prod = gf_mult_poly(q, x1, deg(q, 2*t)+1, deg(x1, 2*t)+1);
            
            printf("PRINTING PROD\n");
            print_arr(prod, deg(prod, 2*t)+1);
            int* x = gf_poly_add(y1, prod, deg(y1, 2*t)+1, deg(prod, 2*t)+1);
            int* y = x1;

            int** ret_val = malloc(sizeof(int*)*3);

            //print_arr(x, deg(x, 2*t)+1);
            //print_arr(y, deg(y, 2*t)+1);

            for(int i = deg(x, 2*t); i < 2*t; i++){
                if(i > deg(x, 2*t)){
                    x[i] = 0;
                }else{
                    x[i] = x[i];
                }
            }
            for(int i = deg(y, 2*t); i < 2*t; i++){
                if(i > deg(y, 2*t)){
                    y[i] = 0;
                }else{
                    y[i] = y[i];
                }
            }
            for(int i = deg(gcd, 2*t); i < 2*t; i++){
                if(i > deg(gcd, 2*t)){
                    gcd[i] = 0;
                }else{
                    gcd[i] = gcd[i];
                }
            }

            ret_val[0] = gcd;
            ret_val[1] = x;
            ret_val[2] = y;
            printf("CONDITION 1 COMPLETE\n");
            print_arr(gcd, deg(gcd, 2*t)+1);
            print_arr(x, deg(x, 2*t)+1);
            print_arr(y, deg(y, 2*t)+1);

            free(tmp_holder);
            return ret_val;
        }
        if(error_msg_y == 1 && error_msg_x == 0){
            printf("GOOD Y BAD X\n");
            int* q = gf_div_poly(a, b, deg(a,a_len)+1, deg(b,b_len)+1,0);
            int prod[1] = {0};

            int** ret_val = malloc(sizeof(int*)*3);

            print_arr(x1, deg(x1, 2*t)+1);
            print_arr(y1, deg(y1, 2*t)+1);

            ret_val[0] = gcd;
            ret_val[1] = y1;
            ret_val[2] = x1;
            printf("HERE\n");

            free(tmp_holder);
            return ret_val;

        }

        if(error_msg_x && error_msg_y == 0){
            printf("GOOD X BAD Y\n");
            int* q = gf_div_poly(a, b, deg(a,a_len)+1, deg(b,b_len)+1,0);
            int* prod = gf_mult_poly(q, x1, deg(q, 2*t)+1, deg(x1, 2*t)+1);

            int* x = prod;
            int* y = x1;

            print_arr(x, deg(x, 2*t)+1);
            print_arr(y, deg(y, 2*t)+1);

            int** ret_val = malloc(sizeof(int*)*3);

            ret_val[0] = gcd;
            ret_val[1] = x;
            ret_val[2] = y;

            free(tmp_holder);
            return ret_val;

        }

        int** ret_val = 0;
        return ret_val;
        
    }


}


int calculate_S(int* R, int num, int n){
    int ret = 0;
    for(int i = 0; i < n; i++){
        ret ^= gf_mult(R[i], gf_pow(gf_pow(generator,num), i));
    }
    return ret;
}


int deg(int* poly, int size){
    if(poly == 0){
        return 0;
    }
    for(int i =size-1; i >= 0; i--){
        if(poly[i] != 0){
            return i;
        }
    }
    return 0;
}


int print_arr(int* a, int len ){
    if(a == 0){
        return 0;
    }
    for(int i = 0; i < len; i++){
        printf("%u*x^%d + \t", a[i], i);
    }
    printf("\n");
    return 1;
}

int parity_check(int* C, int* g, int len){
    for(int i =0; i < len; i++){
        if(C[i] != g[i]){
            printf("C: %d\t G: %d\t i: %d\n", C[i], g[i], i);
            return 0;
        }
    }
    return 1;
}

int* x_pow(int pow){
    if(pow == 0){
        int* poly_ = malloc(sizeof(int));
        poly_[0] = 1;
        return poly_;
    }
    
    int* poly  = malloc(sizeof(int)*(pow+1));
    
    for(int i = 0; i < pow+1; i++){
        if(i == pow){
            poly[i] = 1;
        }else{
            poly[i] = 0;
        }
    }

    return poly;
}

int eval_poly(int* poly, int len, int x){
    int num = 0;
    for(int i =0; i < len; i++){
        num ^= gf_mult(poly[i], gf_pow(x, i));
    }
    return num;
}

void decode(int* C, int k, int n){
    int* mesg = malloc(sizeof(int)*k);
    int mesg_pos = 0;
    for(int i =n-1; i>=n-k; i--){
        mesg[mesg_pos] = C[i];
        mesg_pos+=1;
    }
    printf("Message: ");
    for(int j = 0; j < k; j++){
        bin(mesg[j]);
    }
    printf("\n");
    printf("Message: ");
    for(int j = 0; j < k*8; j++){
        printf("%d", bits[j]-'0');
    }
    printf("\n");
}


int main(){
    setup_tables();

    int n = 10;
    int k = 3;
    //int d = n-k+1;
    int t = (n-k)/2;

    //local data to show encoding works
    encode_message(n,k,t);



    //int check = parity_check(C,C,n);

    //if(parity_check){
    //decode(C,k,n);
    //}

    //printf("Parity Check: %d\n", check);

    //Doesn't work
    //int* S_i_s = syndome_calculator_collective_sums(C, n, t);
    
    //print_arr(S_i_s, 2*t);


    //encode_message(n,k,t);

    return 0;
}






int setup_tables(){
    unsigned int x_to_w=0; 
    unsigned int b=0; 
    unsigned int log = 0;

    x_to_w =  NW;
    gflog = malloc(sizeof(unsigned int) * x_to_w);
    gfilog = malloc(sizeof(unsigned int) * x_to_w);

    b = 1;
    for(log = 0; log < x_to_w-1; log++){
        gflog[b] = (unsigned int)log;
        gfilog[log] = (unsigned int) b;
        b = b << 1;
        if(b & (x_to_w)){
            // printf("log: %d\n", log);
            // bin(b);
            // printf("\n\n");
            // bin(prim_poly);
            // printf("\n\n");
            // bin(x_to_w);
            // printf("\n\n");
            // bin(b ^ prim_poly);
            // printf("\n");
            b = (b ^ prim_poly) ;
            //bin(b);
        }

    }

    //for(int i = 0; i < NW; i++){
    //    printf("%d\t",gflog[i]);
    //}

    //printf("\n");

    //exit(1);

    return 0;

}


int gf_mult(int a, int b){
    int sum_log;

    if(a == 0 || b == 0) return 0;
    sum_log = (int)gflog[a] + (int)gflog[b];
    if(sum_log >= NW-1){ 
        sum_log -= NW-1;
    }
    //printf("%d\n", sum_log);
    return gfilog[sum_log];
}

int* gf_mult_poly(int* a, int* b, int a_len, int b_len){

    if(a_len == 1 && b_len == 1){
        int* ret = malloc(sizeof(int)*2);
        ret[0] = gf_mult(a[0], b[0]);
        ret[1] = 0;
        return ret;
    }

    int new_poly_size = deg(a, a_len) + deg(b, b_len);

    int* new_poly = malloc(sizeof(int)*new_poly_size);
    
    for(int i =0; i < new_poly_size; i++){
        new_poly[i]= (unsigned int) 0;
    }

    for(int i = a_len-1; i >= 0; i--){
        for(int j = b_len-1; j >= 0; j--){
            if(a[i] > NW || b[j] > NW) { print_arr(a, a_len); print_arr(b,b_len); printf("A_LEN=%d\t B_LEN=%d\t NEW_SIZE=%d\n", a_len, b_len, new_poly_size);}
            //printf("INSIDE DIS SHIT\n");
            if(new_poly[i+j] == 0) {new_poly[i+j] = (unsigned int)gf_mult(a[i], b[j]);
            }else{new_poly[i+j] ^= (unsigned int)gf_mult(a[i], b[j]);}
            
        }
    }

    return new_poly;
}

int gf_div(int a, int b){
    int diff_log;

    if(a == 0) return 0;
    if(b == 0) return 0;
    diff_log = (int)gflog[a] - (int)gflog[b];
    if(diff_log < 0) diff_log += NW-1;
    return gfilog[diff_log];
}

int* gf_poly_add(int* a, int* b, int len, int len2){
    int max = 0;
    int min = 0;
    if(len > len2){
        max = len;
        min = len2;
    }else{
        max = len2;
        min = len;
    }

    int* poly = malloc(sizeof(int)*max);

    for(int i =0; i < max; i++){
        if(i >= min){
            if(min == len2){
                poly[i] = a[i];
            }else{
                poly[i] = b[i];
            }
        }else{
            poly[i] = a[i] ^ b[i];
        }
    }
    return poly;
}

// a/b if remainder = 1 return remainder else return the divsion
int* gf_div_poly(int* a, int* b, int a_len, int b_len, int remainder){
    
    int* zero = malloc(sizeof(int)*1);
    zero[0] = 0;
    if(a_len == 0 || b_len == 0) return zero;

    if(a_len ==1){
        int div = a[0];
        for(int i = 0; i < b_len; i++){
            b[i] = gf_div(div, b[i]);
        }
        return b;
    }
    
    if(b_len == 1){
        return zero;
    }

    int range = abs( a_len - b_len - 1);
    int pos = 0;

    //printf("%d\n", range);

    int* divisor = malloc(sizeof(int)*b_len);

    int* dividend = malloc(sizeof(int)*a_len);

    for(int i =0; i < b_len; i++){
        divisor[i] = b[i];
 
    }
    for(int i =0; i < a_len; i++){
        dividend[i] = a[i];
 
    }

    int* r = malloc(sizeof(int)*(a_len));
    int* q = malloc(sizeof(int)*a_len);

    int div_location = b_len-1;
    int a_pos = a_len-1;
    int i = deg(dividend,a_len);

    while(i >= b_len-1){
        int coeff = gf_div(dividend[i], divisor[b_len-1]);
        //printf("Coeff = %d a=%d\t b=%d\t div_location=%d\t pos=%d \t i=%d\t b_len-1=%d\n", coeff, dividend[i], divisor[b_len-1],div_location,pos,i,b_len-1);
        //print_arr(dividend, a_len); 
        //print_arr(divisor, b_len); 
        q[(i-b_len+1)] = coeff;


        if(coeff != 0){
            int tmp_pos= 0;
            for(int j = b_len-1; j >= 0;j--){
                if (divisor[j] != 0){
                    //printf("a=%d  b=%d b/a=%d range=%d\n", dividend[j], divisor[j], gf_div(coeff, divisor[j]), range);
                    //printf("product = %d\n", gf_mult(divisor[j], coeff));
                    dividend[i-tmp_pos] ^= gf_mult(divisor[j], coeff);
                    //printf("x^%d + x^%d \n", i-tmp_pos, j);
                }
                tmp_pos++;
            }
            //print_arr(dividend, a_len); 
            div_location--;
        }
        pos++;
        i = deg(dividend,a_len);

    }
    //print_arr(q, a_len);



    int sep = b_len - 1;

    if(remainder == 1){
        //printf("POSITON1\n");
        //printf("a_len=%d \t b_len=%d \t set=%d \t range=%d\n", a_len, b_len, sep, range);
        free(r);
        free(divisor);
        free(zero); 
        return dividend;
    }else{
        //printf("POSITION\n");
        free(r);
        free(zero);
        free(divisor);
        free(dividend);
        return q;
    }

}

int gf_pow(unsigned int a, int b){
    unsigned int ret = 1;
    for (int i =0; i < b; i++){
        ret = (unsigned int) gf_mult(ret, a);
        //printf("ret=%d\t a=%d\t b=%d\n", ret, a, b);
        //sleep(1);
    }
    return ret;
}

void bin(unsigned n) 
{ 
    unsigned i; 
    for (i = 1 << 16; i > 0; i = i / 2) 
        (n & i)? printf("1"): printf("0");
   //printf("\n"); 
} 

unsigned int str_int(char* str){
    int tmp_num = 0;
    unsigned int num = 0;
    int size = 7;

    for (int i =0; i <= size; i++){
        if(str[i] == '0'){
            tmp_num = 0;
        }else{
            tmp_num = 1;
        }
        num += (unsigned int)tmp_num*((int)pow(2, size-(i)));   
    }

    return num;
}

int gf_inverse(int num){

    for(int i = 1; i < NW; i++){
        if(gf_mult(num,i) == 1){
            return i;
        }
    }

    return 0;

}
